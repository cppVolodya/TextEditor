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

void TextEditor::OnActionOpenTriggered()
{
	const QString path_to_file{QFileDialog::getOpenFileName(this, "Open")};
	if (not path_to_file.isEmpty())
	{
		this->m_current_filename = path_to_file;
		QFile input_file{path_to_file};
		if (!input_file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			(void)QMessageBox::warning(this,
									   "Warning",
									   "Failed to open file for reading data: " + input_file.errorString());
		}

		this->m_ui->text_edit->setPlainText(input_file.readAll());

		const auto position{path_to_file.lastIndexOf(QChar('/')) + 1};
		const QString filename{path_to_file.right(path_to_file.size() - position)};
		this->setWindowTitle(filename + " - Notepad");
	}
}

void TextEditor::OnActionSaveTriggered()
{
	if (this->m_current_filename.isEmpty())
	{
		QString path_to_file{QFileDialog::getSaveFileName(this, "Save")};
		if (not path_to_file.isEmpty())
		{
			this->m_current_filename = path_to_file;
			QFile output_file{path_to_file};
			if (!output_file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				(void)QMessageBox::warning(this,
										   "Warning",
										   "Could not open file for writing data: " + output_file.errorString());
			}

			const auto number_of_bytes{output_file.write(this->m_ui->text_edit->toPlainText().toLocal8Bit())};
			if (number_of_bytes == -1)
			{
				(void)QMessageBox::warning(this,
										   "Warning",
										   "Could not write data to file: " + output_file.errorString());
			}
		}
	}
	else
	{
		QFile output_file{this->m_current_filename};
		if (!output_file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			(void)QMessageBox::warning(this,
									   "Warning",
									   "Could not open file for writing data: " + output_file.errorString());
		}

		const auto number_of_bytes{output_file.write(this->m_ui->text_edit->toPlainText().toLocal8Bit())};
		if (number_of_bytes == -1)
		{
			(void)QMessageBox::warning(this,
									   "Warning",
									   "Could not write data to file: " + output_file.errorString());
		}
	}
}

void TextEditor::OnActionFontTriggered()
{
	bool check{true};
	QFont font = QFontDialog::getFont(&check, QFont(), this, "Font...");

	if (check) {
		this->m_ui->text_edit->setCurrentFont(font);
	}
}

void TextEditor::ZoomInTextEditorScale()
{
	if (StatusBar::S_MAX_SCALE_VALUE_FOR_TEXT_EDITOR > this->m_status_bar->GetTextEditorScaleValue())
	{
		this->m_ui->text_edit->zoomIn();
		emit this->m_status_bar->TextEditorZoomSizeChanged(TextEditor::S_STEP_ZOOM_IN);
	}
}

void TextEditor::ZoomOutTextEditorScale()
{
	if (StatusBar::S_MIN_SCALE_VALUE_FOR_TEXT_EDITOR < this->m_status_bar->GetTextEditorScaleValue())
	{
		this->m_ui->text_edit->zoomOut();
		emit this->m_status_bar->TextEditorZoomSizeChanged(TextEditor::S_STEP_ZOOM_OUT);
	}
}
}  // namespace N_TextEditor
