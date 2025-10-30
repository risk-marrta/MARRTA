#include "eventhomepage.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

EventHomePage::EventHomePage(QString newCode)
{
    setTitle("Para la creación de un suceso iniciador debe rellenar la siguiente información");

    QGridLayout *layout = new QGridLayout;
    // Add margins to prevent text from being too close to the edges
    layout->setContentsMargins(10, 10, 20, 10); // left, top, right, bottom
    
    auto labelName = new QLabel("Nombre");
    auto editName = new QLineEdit;
    auto labelCode = new QLabel("Código");
    auto editCode = new QLineEdit;
    editCode->setText(newCode);
    auto labelDescription = new QLabel("Descripción");
    auto editDescription = new QPlainTextEdit;
    editDescription->setMinimumHeight(100);
    auto cbHuman = new QCheckBox("Es un error humano");

    layout->addWidget(labelName, 0, 0);
    layout->addWidget(editName, 0, 1);
    layout->addWidget(labelCode, 1, 0);
    layout->addWidget(editCode, 1, 1);
    layout->addWidget(labelDescription, 2, 0);
    layout->addWidget(editDescription, 2, 1);
    layout->addWidget(cbHuman, 3, 0, 1, 2);
    setLayout(layout);
    registerField("name*", editName);
    registerField("code", editCode);
    registerField("description", editDescription, "plainText");
    registerField("human", cbHuman);
}
