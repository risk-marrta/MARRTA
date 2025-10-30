#ifndef ORDERABLEWIDGET_H
#define ORDERABLEWIDGET_H

#include <QList>
#include <QMessageBox>
#include <memory>
#include <iostream>
#include "orderable.h"

/**
 * Change the order between two elements
 */
template <class T> void changeOrder(QList<T> &elements, int id, bool increment)
{
    int searchedOrder = increment ?
        elements[id].orden + 1 :
        elements[id].orden - 1;
    for(auto &stage : elements) {
        if (stage.orden == searchedOrder) {
            stage.orden = elements[id].orden;
            stage.modificado = true;
            break;
        }
    }
    elements[id].orden = searchedOrder;
    elements[id].modificado = true;
};

/**
 * Add a element to the list of elements.
 */
template <class T> void addOrderableElement(QList<T> &elements, T &element)
{
    element.id = elements.empty() ? 1 : elements.last().id + 1;
    element.creado = true;
    element.orden = elements.empty() ? 0 : elements.last().orden + 1;
    //std::cout << elements.size() << std::endl;
    elements << element;
    //std::cout << elements.size() << std::endl;
    //std::cout << elements.back().orden << std::endl;
}

/**
 * Call user to remove the selected element
 *
 * @param QList<T>& elements Elements that store the data
 * @param int       id       Id of the selected element
 * @param bool&     reset    If the element's list has been changed
 *
 * @returns
 *  - std::shared_ptr<T> With the erased element of null
 *  - bool               If the container should be reset (the element is not been recently created)
 */
template <class T> std::tuple<std::shared_ptr<T>, bool> removeOrderableElement(QList<T> &elements, int id)
{
    bool reset = false;
    std::shared_ptr<T> toDelete(nullptr);

    QMessageBox msgBox;
    msgBox.setText("¿Realmente desea eliminar este elemento?");
    msgBox.setInformativeText("Después de borrar debe guardar para que los cambios se hagan efectivos");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes) {
        for(int i = 0; i < elements.length(); i++) {
            if (elements[i].id == id) {
                if (elements[i].creado == false) {
                    toDelete.reset(new T(elements[i]));
                    elements.removeAt(i);
                }
                reset = true;
            }
        }
    }

    return {toDelete, reset};
}

#endif // ORDERABLEWIDGET_H
