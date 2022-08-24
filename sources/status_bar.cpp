// Copyright [2022] <Volodymyr Dorozhovets>"

#include "status_bar.hpp"


namespace N_TextEditor
{
StatusBar::StatusBar(QWidget *parent)
	: QStatusBar(parent),
	  m_text_editor_scale_value(100)
{
	this->m_emptiness_label 		= std::make_unique<QLabel>(			    ).release();
	this->m_cursor_position_label 	= std::make_unique<QLabel>("Ln 1, Col 1").release();
	this->m_text_editor_scale_label = std::make_unique<QLabel>("100%"		).release();

	this->addPermanentWidget(this->m_emptiness_label, 		  10);
	this->addPermanentWidget(this->m_cursor_position_label,    2);
	this->addPermanentWidget(this->m_text_editor_scale_label,  1);

	this->setStyleSheet("QStatusBar"
						"{"
						"border-top-width: 1px    ;"
						"border-top-style: solid  ;"
						"border-top-color: #d7d7d7;"
						"}");
}

void StatusBar::SetValueForCursorPositionOnLabel(const QTextCursor &text_cursor)
{
	this->m_cursor_position_label->setText("Ln " 	+ QString::number(text_cursor.blockNumber () + 1) +
										   ", Col " + QString::number(text_cursor.columnNumber() + 1));
}

void StatusBar::SetScaleValueForTextEditor(const int value)
{
	this->m_text_editor_scale_value += value;
	this->m_text_editor_scale_label->setText(QString::number(this->m_text_editor_scale_value) + "%");
}
}  // namespace N_TextEditor
