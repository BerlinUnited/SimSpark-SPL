#ifndef SETTINGSTREEVIEW_H
#define SETTINGSTREEVIEW_H
/**
 * \file settingstreeview.h
 * \brief Contains SettingsTreeView Class, to display setting data of a SettingsTreeModel.
 */
 
#include <QTreeView>
#include <boost/shared_ptr.hpp>

class QSettings;

/**
    \brief Class for viewing QSettings from a model.
*/
class SettingsTreeView : public QTreeView
{
    Q_OBJECT
 
public:
    //public functions
 
    /*!
     \brief Constructor.
    */
    SettingsTreeView(QWidget* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~SettingsTreeView();
 
private:
    //private members
 
}; 

#endif //SETTINGSTREEVIEW_H
