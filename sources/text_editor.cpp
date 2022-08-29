// Copyright [2022] <Volodymyr Dorozhovets>"

#include "text_editor.hpp"


namespace N_TextEditor
{
TextEditor::TextEditor(QWidget *parent)
	: QMainWindow(parent),
	  m_ui(std::make_unique<Ui::TextEditor>().release())
{
	this->m_ui->setupUi(this);
	this->m_ui->text_edit->viewport()->installEventFilter(this);

	this->m_status_bar = std::make_unique<StatusBar>().release();
	this->setStatusBar(this->m_status_bar);

	this->m_shortcut_ctrl_plus 			   = std::make_unique<QShortcut>(QKeySequence(Qt::CTRL | Qt::Key_Plus),  this).release();
	this->m_shortcut_alternative_ctrl_plus = std::make_unique<QShortcut>(QKeySequence(Qt::CTRL | Qt::Key_Equal), this).release();
	this->m_shortcut_ctrl_minus 		   = std::make_unique<QShortcut>(QKeySequence(Qt::CTRL | Qt::Key_Minus), this).release();

	(void)connect(this->m_ui->action_new, &QAction::triggered,
				  this,                   &TextEditor::OnActionNewTriggered);
	(void)connect(this->m_ui->action_open, &QAction::triggered,
				  this,                    &TextEditor::OnActionOpenTriggered);
	(void)connect(this->m_ui->action_save, &QAction::triggered,
				  this,                    &TextEditor::OnActionSaveTriggered);

	(void)connect(this->m_ui->action_exit, &QAction::triggered,
				  this,                    &QApplication::quit);
	(void)connect(this->m_ui->action_cut, &QAction::triggered,
				  this,                   &TextEditor::OnActionCutTriggered);
	(void)connect(this->m_ui->action_undo, &QAction::triggered,
				  this,                    &TextEditor::OnActionUndoTriggered);
	(void)connect(this->m_ui->action_copy, &QAction::triggered,
				  this,                    &TextEditor::OnActionCopyTriggered);
	(void)connect(this->m_ui->action_paste, &QAction::triggered,
				  this,                     &TextEditor::OnActionPasteTriggered);
	(void)connect(this->m_ui->action_delete, &QAction::triggered,
				  this,                      &TextEditor::OnActionDeleteTriggered);

	(void)connect(this->m_ui->action_font, &QAction::triggered,
				  this, 				   &TextEditor::OnActionFontTriggered);
	(void)connect(this->m_ui->action_syntax_highlighting, &QAction::triggered,
				  this, 								  &TextEditor::OnActionSyntaxHighlightingTriggered);

	(void)connect(this->m_ui->text_edit,   &QTextEdit::undoAvailable,
				  this->m_ui->action_undo, &QAction::setEnabled);
	(void)connect(this->m_ui->text_edit,  &QTextEdit::copyAvailable,
				  this->m_ui->action_cut, &QAction::setEnabled);
	(void)connect(this->m_ui->text_edit,   &QTextEdit::copyAvailable,
				  this->m_ui->action_copy, &QAction::setEnabled);

	(void)connect(this->m_ui->text_edit, SIGNAL(cursorPositionChanged()),
				  this, 				 SLOT  (CurrentCursorPosition()));
	(void)connect(this, 			  SIGNAL(CursorPositionChanged			 (QTextCursor)),
				  this->m_status_bar, SLOT  (SetValueForCursorPositionOnLabel(QTextCursor)));

	(void)connect(this->m_status_bar, SIGNAL(TextEditorZoomSizeChanged (int)),
				  this->m_status_bar, SLOT  (SetScaleValueForTextEditor(int)));

	(void)connect(this->m_shortcut_ctrl_plus, &QShortcut::activated,
				  this, 					  &TextEditor::ZoomInTextEditorScale);
	(void)connect(this->m_shortcut_alternative_ctrl_plus, &QShortcut::activated,
				  this, 								  &TextEditor::ZoomInTextEditorScale);
	(void)connect(this->m_shortcut_ctrl_minus, &QShortcut::activated,
				  this,						   &TextEditor::ZoomOutTextEditorScale);
}

void TextEditor::wheelEvent(QWheelEvent *wheel_event)
{
	if (wheel_event->modifiers() == Qt::ControlModifier)
	{
		if (wheel_event->angleDelta().ry() > 0)
		{
			this->ZoomInTextEditorScale();
		}
		else
		{
			this->ZoomOutTextEditorScale();
		}
	}
}

bool TextEditor::eventFilter(QObject *object, QEvent *event)
{
	const auto wheel_event{dynamic_cast<QWheelEvent *>(event)};

	bool is_handled{false};
	if (object == this->m_ui->text_edit->viewport() &&
		event->type() == QEvent::Wheel 				&&
		wheel_event->modifiers() == Qt::ControlModifier)
	{
		wheelEvent(wheel_event);
		is_handled = true;
	}

	return is_handled;
}
}  // namespace N_TextEditor
