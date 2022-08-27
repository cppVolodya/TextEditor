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
signals:
	void CursorPositionChanged(const QTextCursor &text_cursor);
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
private slots:
	inline void OnActionNewTriggered();
	void OnActionOpenTriggered();
	void OnActionSaveTriggered();

	inline void OnActionCutTriggered();
	inline void OnActionUndoTriggered();
	inline void OnActionCopyTriggered();
	inline void OnActionPasteTriggered();
	inline void OnActionDeleteTriggered();

	void OnActionFontTriggered();
	inline void OnActionSyntaxHighlightingTriggered();

	inline void CurrentCursorPosition();

	void ZoomInTextEditorScale();
	void ZoomOutTextEditorScale();
};

inline TextEditor::~TextEditor()
{
	delete this->m_ui;
}

inline void TextEditor::OnActionNewTriggered()
{
	this->m_ui->text_edit->clear();
	this->setWindowTitle("Untitled - Notepad");
	this->m_current_filename.clear();
}

inline void TextEditor::OnActionUndoTriggered()
{
	this->m_ui->text_edit->undo();
}

inline void TextEditor::OnActionCutTriggered()
{
	this->m_ui->text_edit->cut();
}

inline void TextEditor::OnActionCopyTriggered()
{
	this->m_ui->text_edit->copy();
}

inline void TextEditor::OnActionPasteTriggered()
{
	this->m_ui->text_edit->paste();
}

inline void TextEditor::OnActionDeleteTriggered()
{
	QKeyEvent *key_press{std::make_unique<QKeyEvent>(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier).release()};
	QApplication::postEvent(this->m_ui->text_edit, key_press);
}

inline void TextEditor::OnActionSyntaxHighlightingTriggered()
{
	this->m_ui->text_edit->setTextColor(QColorDialog::getColor(QColor(), this, "Syntax highlighting"));
}

inline void TextEditor::CurrentCursorPosition()
{
	emit this->CursorPositionChanged(this->m_ui->text_edit->textCursor());
}
}  // namespace N_TextEditor

#endif  // MAIN_WINDOW_H
