/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef SPARKPROPERTY_H
#define SPARKPROPERTY_H
/**
 * \file sparkproperty.h
 * \brief Contains SparkProperty::ClassDescriptor and SparkProperty::Property classes as well as enumerations and typedefs.
 */

#include "sparkpropertydata.h"

#include <map>
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#include <QString>
#include <vector>
#include <QSemaphore>

#include <salt/vector.h>
#include <salt/bounds.h>
#include <salt/matrix.h>
#include <kerosin/materialserver/materialsolid.h>

namespace zeitgeist
{
    class Class;
    class Leaf;
}

class QValidator;

namespace SparkProperty
{

class PropertyList;
class PropertyControl;
class Property;
class Data;

/*!
 \brief Describes a single class to generate Properties for.
*/
struct ClassDescriptor
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param name name of the class
     \param id freely defineable id of the class
     \param defaultControl property control that handles this class
    */
    ClassDescriptor(const QString& name = QString(""), int id = -1, PropertyControl* defaultControl = 0) :
      mName(name), mId(id), mDefaultControl(defaultControl) {}
    /*!
     \brief Destructor.
    */
    ~ClassDescriptor(){}

    QString mName; /*!< Name of the class. */
    int mId; /*!< Freely defineable id of the class. */
    PropertyControl* mDefaultControl; /*!< Property control that handles this class. */
};

/*!
 \brief Flag signaling if a property is currently being updated.

 Properties that are being read can be read again.
 Properties that are to be written update the value to write if written again.
 Properties that are to be written/read cant be read/written until updating is over.
*/
enum EUpdateFlag
{
    UF_NONE = 0, /*!< Property inactive. */
    UF_READING,  /*!< Currently updating (reading) property. */
    UF_WRITING   /*!< Currently writing (applying) property change into SimSpark. */
};

/*!
 \brief List of class descriptors used in PropertyControl and the PropertyManager.
*/
typedef std::vector<ClassDescriptor> TClassList;
/*!
 \brief List of property objects.
*/
typedef std::vector<boost::shared_ptr<Property>> TProperties;
/*!
 \brief Helper type for zeitgeist leaf shared ptr.
*/
typedef boost::shared_ptr<zeitgeist::Leaf> TLeafPtr;
/*!
 \brief Helper type for zeitgeist class shared ptr.
*/
typedef boost::shared_ptr<zeitgeist::Class> TClassPtr;

/*!
 \brief Class for a single property wrapping a read and write access to SimSpark functions.

 <b>General:</b>\n
 Properties are interfaces to functions of a single Class of a single leaf of SimSpark.
 They contain a Data object of a fix data type and can use this Data object to read out and store information from SimSpark,
 as well as editing the value and applying changes by calling functions of SimSpark.
 Properties are contained in a PropertyList and managed by this list and its assigned PropertyManager.
 Properties are created by PropertyControl objects which support a set of SimSpark classes.
 The Property-System as a whole is basically a threadsafe and elaborate read- and write-interface to SimSpark,
 which can be extended and used in different contexts.

 <b>Types:</b>\n
 A Property contains a Data object of a non-changeable data type.
 Some Properites can only be read, others can be read and written.\n

 Properties do not HAVE to be a direct interface to a pair of Set/Get functions of SimSpark.
 They simply display a value returned by a function and allow calling a function with a parameter of this value.
 For example they can also be used as input-field for direct commands to SimSpark, like script commands.
 However, it is adviseable to always make it clear to the User how to interact with a Property.

 <b>Data and Updating:</b>\n
 A Property contains only one data type, but in up to three different versions.
 The "normal" value mValue is the Data object that was created after reading out the Property (calling a function returning a value) from SimSpark.
 The "temporary" value mTempValue is used to specify a new value to be handed to SimSpark.
 When setting a property-value, only the mTempValue will be set.
 By calling startUpdate() with the write-flag the temporary value can be sent to the PropertyControl object that created the
 property, to actually apply the new value by calling a SimSpark function.
 The "string" value mStringValue is a representation of the currently active (temporary if existant or normal if not) value.
 The string value is created by a string conversion function in the Data class.\n

 Read- or Write-Updates for Properties are queued in a command queue of a PropertyExecuter assigned to a PropertyControl.
 Consecutive read-updates will be ignored (only one read update per Property can and has to be queued at most).
 Consecutive write-updates will simply update the temporary value to be set with the write update.
 Read-updates while writing will be ignored, because there is an automatic update after writing.

 <b>Creation:</b>\n
 Properties are created by a SparkProperty::PropertyControl derived class.
 See PropertyControl documentation for more information.

 <b>Initialization, Read and Write:</b>\n
 Initialization of Properties, Read-Updates and Write-Updates are the three sequences that have to be coordinated between different
 participants (PropertyControl, PrpertyList, Property, PropertyWidget, PropertyManager) and different threads (thread displaying the property,
 threads modifying the property, thread that owns the data the Property refers to).
 There are several Semaphores and Techniques to guarantee a safe and fluent execution of all of the involved threads.
 Frequent Read- or write updates will not block the PropertyExecuter thread, nor will there by loss of data because of communication latency.
 See the Sequence-Diagrams in the Wiki for detailed information about how these steps work.

 \todo The Property-System is meant to be just as extendable as the Gui and as SimSpark and offers a solid base to do just that.
 Because of a lack of time several aspects could not be implemented yet. This includes:\n
 - A central PropertyManager that utilizes the CommunicationManager to handle registration of PropertyControl instead of having it as a child of a PropertyList.\n
 - Having the Property-System as independant (Gui-)Plugin instead of it being Gui-Internal.\n
 - Additional Data-Types to support the whole set of SimSpark data types.\n
 - Additional Data-Widgets to be able to comforably edit and work with the data types.\n
 - Additional Validators to be able to utilize better contraints for complex data types like matrices.\n
 - Support for additional basic SimSpark-classes by extending SparkProperty::BasicControl.\n
 - Support for soccer classes by creating a new SparkProperty::SoccerControl.\n
 See the other Property-System classes for more information.

 \sa PropertyControl, PropertyManager, ProptertyList, Property, PropertyWidget, Data
*/
class Property
{
    friend class PropertyManager;
    friend class PropertyControl;
    friend class PropertyList;
    friend class PropertyExecuter;

public:
    //public types

    /*!
     \brief Complete constructor.

     \param leaf leaf to interact with
     \param classDes descriptor for the class to identify the corrent class handler
     \param name name of the property to identify the correct function handler
     \param caption caption to be displayed in the gui
     \param value value of the property, will be owned by the property
     \param editable if true, property is editable, otherwise only readable
     \param validator matching validator for the given data object
     \param help tooltip for the property
    */
    Property(boost::shared_ptr<zeitgeist::Leaf> leaf, ClassDescriptor classDes, const QString& name, const QString& caption,
        Data* value, bool editable = false, QValidator* validator = 0, const QString& help = QString());
    /*!
     \brief Complete constructor.

     \param leaf leaf to interact with
     \param classDes descriptor for the class to identify the corrent class handler
     \param name name of the property to identify the correct function handler
     \param caption caption to be displayed in the gui
     \param value value of the property, will be owned by the property
     \param help tooltip for the property
    */
    Property(boost::shared_ptr<zeitgeist::Leaf> leaf, ClassDescriptor classDes, const QString& name, const QString& caption,
        Data* value, const QString& help);
    /*!
     \brief Destructor.
    */
    ~Property();

    /*!
     \brief Update the mStringValue by converting the currently active value to a string.
    */
    const QString& createStringValue();

    /*!
     \brief Returns the current update flag.
    */
    EUpdateFlag getUpdateFlag() const;
    /*!
     \brief Returns the currently active value.

     If write-updating, returns the temp value. Otherwise returns the normal value.
    */
    const boost::shared_ptr<const Data> getCurrentValue() const;
    /*!
     \brief Returns the last read value, regardless of having a temporary write-value or not.
    */
    const boost::shared_ptr<const Data> getValue() const;
    /*!
     \brief Returns the temporary value to be written. Null pointer if not writing.
    */
    const boost::shared_ptr<const Data> getTempValue() const;
    /*!
     \brief Returns the data type of the Property.
    */
    EDataType getDataType() const;

    /*!
     \brief Set a new property value as temporary value. Pointer will be owned.

     Afterwards, the property has to be updated by updating its string value (createStringValue()) and writing the value to the property list
     with PropertyList::writeProperty().\n

     The data type has to be identical to the properties data type.
     \param data pointer to new data
     \return true if set was successful, false if property access timed out, the value is currently read-updating or an error occurred
    */
    bool setValue(Data* data);
    /*!
     \brief Set a new property value as temporary value.


     Afterwards, the property has to be updated by updating its string value (createStringValue()) and writing the value to the property list
     with PropertyList::writeProperty().\n

     The data type has to be identical to the properties data type.
     \param data pointer to new data
     \return true if set was successful, false if property access timed out, the value is currently read-updating or an error occurred
    */
    bool setValue(boost::shared_ptr<Data> data);
    /*!
     \brief Set a new property value by string.

     Afterwards, the property has to be updated by writing the value to the property list
     with PropertyList::writeProperty().
     Use isValid(QString) beforehand to check if a string value is valid.\n

     The data type has to be identical to the properties data type.
     \param dataString
     \return true if set was successful, false if property access timed out, the value is currently read-updating or an error occurred
    */
    bool setValue(const QString& dataString);

    /*!
     \brief Check if a data object is a valid data for this property.

     Returns true if the given data has the same type than the properties data type and it matches its validator.
     \param data data to compare with
     \return true if valid
    */
    bool isValid(Data* data = 0);
    /*!
     \brief Check if a data string can be converted to a valid data object for this property.

     \param dataString string to convert
     \return true if valid
    */
    bool isValid(QString dataString);

    ClassDescriptor& getClassNc();           /*!< Returns Description for the class the property was created for. Used to identify the corrent PropertyControl.*/
    const ClassDescriptor& getClass() const; /*!< Returns Description for the class the property was created for. Used to identify the corrent PropertyControl.*/
    const QString& getName() const;          /*!< Returns name of the property. Used to identify the corrent PropertyControl function.*/
    const QString& getCaption() const;       /*!< Returns caption of the property. Displayed in the Gui. */
    const QString& getStringValue() const;   /*!< Returns value as string. Has to be created with createDataString() after setting temp value. */
    const QString& getHelp() const;          /*!< Returns help string. Used as tooltip in the Gui. */
    bool isEditable() const;                 /*!< True if property is writeable, false for read-only. */
    const QValidator* getValidator() const;  /*!< Returns validator for string based value change. */
    boost::weak_ptr<zeitgeist::Leaf> getLeaf(); /*!< Returns leaf to interact with. */

protected:
    //protected functions

    /*!
     \brief Set temporary value. Used by PropertyExecuter.

     Protected write does not use semaphore, because it was already acquired by the PropertyExecuter
     \param data new value, will be owned by the property
    */
    void setValueProtected(Data* data);
    /*!
     \brief Set temporary value. Used by PropertyExecuter.

     Protected write does not use semaphore, because it was already acquired by the PropertyExecuter
     \param data new value
    */
    void setValueProtected(boost::shared_ptr<Data> data);

    /*!
     \brief Start read or write updating.

     \param flag type of update
     \return true if update was started, false if error occurred or update is already activated
    */
    bool startUpdate(EUpdateFlag flag);
    /*!
     \brief Notification that update finished.

     If successful, temp value will be accepted, otherwise update flag will simply be resetted.
     \param ok true if update was successful
    */
    void updateFinished(bool ok);
    /*!
     \brief Accept the temporary value. Called by PropertyExecuter.

     Resets update flag. Notifies parent that update finished.
     Updates string value.
    */
    void acceptTempValue();
    /*!
     \brief Reject the temporary value. Called by PropertyExecuter.

     Resets update flag. Notifies parent that update finished.
     Updates string value.
    */
    void rejectTempValue();

    /*!
     \brief Define the parent property list of this property.

     Notification of finished updates will go to this property list.
     \param parent new parent list
    */
    void setParent(PropertyList* parent);

    /*!
     \brief Tries to lock access to the properties values.

     If locking succeeds, unlock has to be called.
     \param time timeout in ms
     \return true if successful, false after timeout
    */
    bool lock(int time);
    /*!
     \brief Unlocks access to the properties values.
    */
    void unlock();

private:
    //private functions.

    /*!
     \brief Notifies parent PropertyList that an update finished.
    */
    void notifyParent();

protected:
    //protected members

    ClassDescriptor mClass; /*!< Description for the class the property was created for. Used to identify the corrent PropertyControl.*/
    QString mName;          /*!< Name of the property. Used to identify the corrent PropertyControl function.*/
    QString mCaption;       /*!< Caption of the property. Displayed in the Gui. */
    QString mStringValue;   /*!< Value of the property as string. */
    QString mHelp;          /*!< Help string. Used as tooltip in the Gui. */
    bool mEditable;         /*!< True if property is writeable, false for read-only. */

    boost::weak_ptr<zeitgeist::Leaf> mLeaf; /*!< Leaf to interact with. */
    QValidator* mValidator; /*!< Validator for string based value change. */
    PropertyList* mParent;  /*!< Parent property list. Defined externally. */

    boost::shared_ptr<Data> mValue;     /*!< Last read value of the property. */
    boost::shared_ptr<Data> mTempValue; /*!< Temporary value while write-updating. */

private:
    //private members

    EUpdateFlag mUpdateFlag;    /*!< Flag representing the current update-status. */
    QSemaphore mValueSemaphore; /*!< Semaphore for access to the properties value. */
};

} //namespace SparkProperty

#endif // SPARKPROPERTY_H
