// Copyright [2022] <Volodymyr Dorozhovets>"

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets>

#include "ui_text_editor.h"
#include "status_bar.hpp"


namespace Ui
{
class TextEditor;
}

namespace N_TextEditor
{
class TextEditor: public QMainWindow
{
Q_OBJECT
public:
	explicit TextEditor(QWidget *parent = nullptr);

	inline ~TextEditor() override;
protected:
	void wheelEvent(QWheelEvent *wheel_event) override;

	bool eventFilter(QObject *object, QEvent *event) override;
private:
	inline static constexpr int S_STEP_ZOOM_IN {+10};
	inline static constexpr int S_STEP_ZOOM_OUT{-10};
private:
	Ui::TextEditor *m_ui;

	StatusBar *m_status_bar;

	QShortcut *m_shortcut_ctrl_plus;
	QShortcut *m_shortcut_alternative_ctrl_plus;
	QShortcut *m_shortcut_ctrl_minus;

	QString m_current_filename;
};

inline TextEditor::~TextEditor()
{
	delete this->m_ui;
}
}  // namespace N_TextEditor

#endif  // MAIN_WINDOW_H
