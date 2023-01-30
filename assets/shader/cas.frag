#version 330 core
precision highp float;

in vec3 i_pixel;
out vec4 o_color;

/******************* uniform fields **************************/
uniform highp samplerBuffer u_triangles;
uniform highp samplerBuffer u_BVHNodes;

uniform vec4 u_bgColor;
uniform vec3 u_eysPos;
uniform int u_triCnt;
uniform int u_BVHNodeCnt;

// inverse of camera matrix, which transform pt in camera coordinate to world coordinate
uniform mat4 u_invViewMat;
// focal length define the distance between virtual viewport and camera
uniform float u_focalLen;
/*************************************************************/


/********************** constant *****************************/
#define TRI_OFFSET      7
#define BVH_OFFSET      4
#define PI              3.1415926
#define INF             114514.0
/*************************************************************/


/********************* definition ****************************/
struct Ray {
    vec3 start;
    vec3 dir;
};
struct Triangle {
    vec3 p0, p1, p2;
    vec3 n0, n1, n2;
    vec3 color;
};
struct BVHNode {
    // firt vec3, x = left, y = right, z is reserved
    int left, right;
    // second vec3, x = n, y = start, z is reserved
    int n, start;
    vec3 AA, BB;
};
struct HitResult {
    bool isHit;
    bool back;
    float distance;
    vec3 color;
    vec3 hitPt;
    vec3 viewDir;
};
/*************************************************************/

/********************* method declaration ********************/
Triangle getTri(int i);
BVHNode getBVHNode(int i);
Ray genRay();
HitResult hitTriArray(Ray ray, int l, int r);
HitResult hitTriangle(Ray ray, Triangle triangle);
float hitAABB(Ray ray, vec3 AA, vec3 BB);
HitResult hitBVH(Ray ray);
/*************************************************************/


/*********************** implementation **********************/
void main() {
    Ray ray = genRay();
    HitResult hit = hitBVH(ray);
    if(hit.isHit) {
        o_color = vec4(hit.color.x, hit.color.y, hit.color.z, 1.f);
    } else {
        o_color = u_bgColor;
    }
}

Triangle getTri(int i) {
    int offset = i * TRI_OFFSET;
    Triangle t;

    t.p0 = texelFetch(u_triangles, offset + 0).xyz;
    t.p1 = texelFetch(u_triangles, offset + 1).xyz;
    t.p2 = texelFetch(u_triangles, offset + 2).xyz;

    t.n0 = texelFetch(u_triangles, offset + 3).xyz;
    t.n1 = texelFetch(u_triangles, offset + 4).xyz;
    t.n2 = texelFetch(u_triangles, offset + 5).xyz;

    t.color = texelFetch(u_triangles, offset + 6).xyz;

    return t;
}

BVHNode getBVHNode(int i) {
    int offset = i * BVH_OFFSET;

    BVHNode node;
    vec3 childInfo = texelFetch(u_BVHNodes, offset + 0).xyz;
    vec3 posInfo = texelFetch(u_BVHNodes, offset + 1).xyz;
    node.AA = texelFetch(u_BVHNodes, offset + 2).xyz;
    node.BB = texelFetch(u_BVHNodes, offset + 3).xyz;

    node.left = int(childInfo.x);
    node.right = int(childInfo.y);
    node.n = int(posInfo.x);
    node.start = int(posInfo.y);

    return node;
}

Ray genRay() {
    Ray ray;
    // u_eysPos is in world coordinate system
    ray.start = u_eysPos;
    // we assume i_pixel is in camera coordinate system, so is u_focalLen. so dst.z must be minus
    // dst is now in camera coordinate system
    vec4 dst = vec4(i_pixel.xy, -u_focalLen, 1.f);
    // use inverse view mat to transform dst into world coordinate system 
    dst = u_invViewMat * dst;
    ray.dir = normalize(dst.xyz - ray.start.xyz);
    return ray;
}

HitResult hitTriArray(Ray ray, int l, int r) {
    HitResult result;
    result.isHit = false;
    result.back = false;
    result.distance = INF;
    if(l > r)
        return result;
    for(int i = l; i <= r; i++) {
        Triangle tri = getTri(i);
        HitResult res = hitTriangle(ray, tri);
        if(res.isHit)  result.isHit = true;
        if(res.distance < result.distance) {
            result.distance = res.distance;
            result.back = res.back;
            result.color = tri.color;
        }
    }
    return result;
}

HitResult hitTriangle(Ray ray, Triangle triangle) {
    HitResult res;
    res.distance = INF;
    res.isHit = false;
    res.back = false;

    vec3 p0 = triangle.p0;
    vec3 p1 = triangle.p1;
    vec3 p2 = triangle.p2;

    vec3 s = ray.start;
    vec3 d = ray.dir;
    vec3 N = normalize(cross(p1-p0, p2-p0));

    // camera is placed in front of screen by default
    // N and ray.dir is on the same side
    if (dot(N, d) > 0.0f) {
        N = -N;
        res.back = true;
    }

    // N is parallel to ray.dir
    if (abs(dot(N, d)) < 0.00001f) return res;

    // by now, ray must hit the plane, compute the distance (t * direction)
    // algorithm referencing to https://blog.csdn.net/weixin_44176696/article/details/119044396?spm=1001.2014.3001.5502
    float t = (dot(N, p0) - dot(s, N)) / dot(d, N);

    // triangle is behind of the eye
    if(t < 0.0005f) return res;

    vec3 pt = s + d * t;

    vec3 c1 = cross(p1 - p0, pt - p0);
    vec3 c2 = cross(p2 - p1, pt - p1);
    vec3 c3 = cross(p0 - p2, pt - p2);
    bool hit1 = (dot(c1, N) > 0.0f && dot(c2, N) > 0.0f && dot(c3, N) > 0.0f);
    bool hit2 = (dot(c1, N) < 0.0f && dot(c2, N) < 0.0f && dot(c3, N) < 0.0f);

    if (hit1 || hit2) {
        res.isHit = true;
        res.distance = t;
        res.hitPt = pt;
        res.viewDir = d;
        res.color = triangle.color;
    }

    return res;
}

float hitAABB(Ray ray, vec3 AA, vec3 BB) {
    vec3 dir = ray.dir;
    if(abs(dir.x) < 0.000001 || abs(dir.y) < 0.000001 || abs(dir.z) < 0.000001) { return -1.f; }

    vec3 invDir = 1.f / dir;
    vec3 far = (BB - ray.start) * invDir;
    vec3 near = (AA - ray.start) * invDir;

    vec3 tMax = max(far, near);
    vec3 tMin = min(far, near);

    float t1 = min(tMax.x, min(tMax.y, tMax.z));
    float t0 = max(tMin.x, max(tMin.y, tMin.z));

    if(t0 < t1 && t0 > 0.f) {
        return t0;
    } else {
        return -1.f;
    }
}

HitResult hitBVH(Ray ray) {
    HitResult result;
    result.isHit = false;
    result.distance = INF;

    int stack[256];
    int sp = 0;
    // root node's index is 0
    stack[sp++] = 0;

    while(sp > 0) {
        int index = stack[--sp];
        BVHNode node = getBVHNode(index);

        // leaf node
        if(node.n > 0) {
            HitResult r = hitTriArray(ray, node.start, node.start + node.n - 1);
            if(r.isHit && r.distance < result.distance) { result = r; }
            continue;
        }

        float d1 = -1.f;
        float d2 = -1.f;
        if(node.left > 0) {
            BVHNode leftChild = getBVHNode(node.left);
            d1 = hitAABB(ray, leftChild.AA, leftChild.BB);
        }
        if(node.right > 0) {
            BVHNode rightChild = getBVHNode(node.right);
            d2 = hitAABB(ray, rightChild.AA, rightChild.BB);
        }

        if(d1 > 0.f && d2 > 0.f) {
            if(d1 < d2) {
                // left should be polled out of stack firstly
                stack[sp++] = node.right;
                stack[sp++] = node.left;
            } else {
                // right should be polled out of stack firstly
                stack[sp++] = node.left;
                stack[sp++] = node.right;
            }
        } else if(d1 > 0.f) {
            stack[sp++] = node.left;
        } else if(d2 > 0.f) {
            stack[sp++] = node.right;
        }
    }// stack over

    return result;
}

/*************************************************************/