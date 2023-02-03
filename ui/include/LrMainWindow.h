#ifndef LR_MAIN_WINDOW_H
#define LR_MAIN_WINDOW_H

#include <QtWidgets/qmainwindow.h>

class LrWidget;

class LrMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	LrMainWindow();

	virtual ~LrMainWindow() = default;

public Q_SLOTS:
	void onRrtBtnClicked();

protected:
	virtual void closeEvent(QCloseEvent* e) override;

private:
	LrWidget *m_mainWidget;
};



#endif // !LR_MAIN_WINDOW_H

