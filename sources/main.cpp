// Copyright [2022] <Volodymyr Dorozhovets>"

#include <QApplication>

#include "text_editor.hpp"


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

	N_TextEditor::TextEditor text_editor;
	text_editor.show();

    return application.exec();
}
