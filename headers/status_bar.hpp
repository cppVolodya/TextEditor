// Copyright [2022] <Volodymyr Dorozhovets>"

#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QtWidgets>


namespace N_TextEditor
{
class StatusBar: public QStatusBar
{
Q_OBJECT

public:
	inline static constexpr int S_MIN_SCALE_VALUE_FOR_TEXT_EDITOR{10};
	inline static constexpr int S_MAX_SCALE_VALUE_FOR_TEXT_EDITOR{500};
public:
	explicit StatusBar(QWidget *parent = nullptr);

	[[nodiscard]] inline int GetTextEditorScaleValue() const;
signals:
	void TextEditorZoomSizeChanged(int);
public slots:
	void SetValueForCursorPositionOnLabel(const QTextCursor & text_cursor);

	void SetScaleValueForTextEditor(int value);
private:
	QLabel *m_emptiness_label;
	QLabel *m_cursor_position_label;
	QLabel *m_text_editor_scale_label;

	int m_text_editor_scale_value;
};

[[nodiscard]] inline int StatusBar::GetTextEditorScaleValue() const
{
	return this->m_text_editor_scale_value;
}
}  // namespace N_TextEditor

#endif  // STATUS_BAR_H
