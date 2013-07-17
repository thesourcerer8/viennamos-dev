/*
 *
 * Copyright (c) 2013, Institute for Microelectronics, TU Wien.
 *
 * This file is part of ViennaMOS     http://viennamos.sourceforge.net/
 *
 * Contact: Josef Weinbub             weinbub@iue.tuwien.ac.at
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */



#include "copy.hpp"
#include "device.hpp"
#include "offload_to_worker.hpp"
#include "keys_quantities.hpp"
#include "keys_units.hpp"

//#include "viennagrid/io/vtk_reader.hpp"
#include "viennagrid/io/netgen_reader.hpp"
#include "viennagrid/algorithm/scale.hpp"
//#include "viennagrid/algorithm/quantity_transfer.hpp"

//#include "viennautils/average.hpp"

//#include "scale_algorithm.hpp"
#include "viennaminimodule.h"
//#include "utils.hpp"

#include <QDebug>

ViennaMiniModule::ViennaMiniModule() : ModuleInterface(this)
{
    // setup a new UI widget and register it with this module
    //
    widget    = new ViennaMiniForm;
    register_module_widget(widget); // takes ownership of the widget - no deleting required

    // setup module specific mechanisms
    //
    QObject::connect(widget, SIGNAL(meshFileEntered(QString const&)), this, SLOT(loadMeshFile(QString const&)));
    QObject::connect(this, SIGNAL(materialsAvailable(MaterialManager::Library&)), widget, SLOT(setMaterialLibrary(MaterialManager::Library&)));


    // create output quantities of this module
    //
    pot_quan_vertex("potential",            "Volt", this->name().toStdString(), VERTEX, SCALAR);
    n_quan_vertex("electron_concentration", "1/m^3", this->name().toStdString(), VERTEX, SCALAR);
    p_quan_vertex("hole_concentration",     "1/m^3", this->name().toStdString(), VERTEX, SCALAR);
    pot_quan_cell("potential",            "Volt", this->name().toStdString(), CELL, SCALAR);
    n_quan_cell("electron_concentration", "1/m^3", this->name().toStdString(), CELL, SCALAR);
    p_quan_cell("hole_concentration",     "1/m^3", this->name().toStdString(), CELL, SCALAR);

    // register output quantities of this module in the framework
    //
    register_quantity(pot_quan_vertex);
    register_quantity(n_quan_vertex);
    register_quantity(p_quan_vertex);
    register_quantity(pot_quan_cell);
    register_quantity(n_quan_cell);
    register_quantity(p_quan_cell);
}

ViennaMiniModule::~ViennaMiniModule()
{
}

QString ViennaMiniModule::name()
{
    return "ViennaMini";
}

QString ViennaMiniModule::description()
{
    return "Classical Device Simulator";
}

QString ViennaMiniModule::version()
{
    return "1.0";
}

// function is called after the framework forwarded
// the base data to the module, e.g., material manager
// so in this case we are lettings our UI know, that the
// material data is available
//
void ViennaMiniModule::preprocess()
{
    emit materialsAvailable(material_manager->getLibrary());
}

void ViennaMiniModule::render(Quantity& quan, int step)
{
    Render3D* render = multiview->getCurrentRender3D();

    // if current is actually a render view, and not a, for instance, chart view
    if(render)
    {
        render->update_render_domain();

        if((quan == pot_quan_vertex) || (quan == pot_quan_cell))
          multiview->setCurrentLogScale(false);
        else
          multiview->setCurrentLogScale(true);

        render->color_quantity(quan.name, viennamos::generateDisplayName(quan.name, quan.unit), quan.cell_level);
        render->update();
    }
}

std::size_t ViennaMiniModule::quantity_sequence_size(std::string quankey)
{
//    if(device_id == Device22u::ID())
//    {
//        Device22u* device = database->value<Device22u*>(Device22u::ID());
//        typedef Device22u::Quantities    QuantitiesT;
//        QuantitiesT& quantities = device->quantities();
//        QuantitiesT::iterator it = quantities.find(quankey);
//        return (it->data_sequence).size();
//    }
//    else if(device_id == Device33u::ID())
//    {
//        Device33u* device = database->value<Device33u*>(Device33u::ID());
//        typedef Device33u::Quantities    QuantitiesT;
//        QuantitiesT& quantities = device->quantities();
//        QuantitiesT::iterator it = quantities.find(quankey);
//        return (it->data_sequence).size();
//    }
//        else return 0;
}

bool ViennaMiniModule::is_ready()
{
    return true;
}

void ViennaMiniModule::update()
{

}

void ViennaMiniModule::reset()
{

}

/**
 * @brief
 * @param
 */
void ViennaMiniModule::execute()
{
    DeviceParameters& parameters = widget->getParameters();

    if((device_id == viennamos::Device2u::ID()) && (has<viennamos::Device2u>()))
    {
        viennamos::Device2u& device = access<viennamos::Device2u>();
        ViennaMiniWorker* worker = new ViennaMiniWorker(&device, material_manager->getLibrary(), parameters,
                                                        pot_quan_vertex, n_quan_vertex, p_quan_vertex,
                                                        pot_quan_cell, n_quan_cell, p_quan_cell);
        viennamos::offload(worker, messenger, SIGNAL(finished()), this, SLOT(transferResult()));
    }
    else
    if((device_id == viennamos::Device3u::ID()) && (has<viennamos::Device3u>()))
    {
        viennamos::Device3u& device = access<viennamos::Device3u>();
        ViennaMiniWorker* worker = new ViennaMiniWorker(&device, material_manager->getLibrary(), parameters,
                                                        pot_quan_vertex, n_quan_vertex, p_quan_vertex,
                                                        pot_quan_cell, n_quan_cell, p_quan_cell);
        viennamos::offload(worker, messenger, SIGNAL(finished()), this, SLOT(transferResult()));
    }
}

//
//

void ViennaMiniModule::transferResult()
{
  if((device_id == viennamos::Device2u::ID()) && (has<viennamos::Device2u>()))
  {
    viennamos::Device2u& device = access<viennamos::Device2u>();
    viennamos::copy(device, pot_quan_vertex, multiview);
    viennamos::copy(device, n_quan_vertex,   multiview);
    viennamos::copy(device, p_quan_vertex,   multiview);
    viennamos::copy(device, pot_quan_cell,   multiview);
    viennamos::copy(device, n_quan_cell,     multiview);
    viennamos::copy(device, p_quan_cell,     multiview);
  }
  else
  if((device_id == viennamos::Device3u::ID()) && (has<viennamos::Device3u>()))
  {
    viennamos::Device3u& device = access<viennamos::Device3u>();
    viennamos::copy(device, pot_quan_vertex, multiview);
    viennamos::copy(device, n_quan_vertex,   multiview);
    viennamos::copy(device, p_quan_vertex,   multiview);
    viennamos::copy(device, pot_quan_cell,   multiview);
    viennamos::copy(device, n_quan_cell,     multiview);
    viennamos::copy(device, p_quan_cell,     multiview);
  }
  emit finished();
}



void ViennaMiniModule::loadMeshFile(QString const& filename)
{
    meshfile = filename;

    QString suffix = QFileInfo(filename).suffix();

    if(suffix == "mesh")
    {
        QString type = widget->getMeshType();

        if(type == viennamos::key::vdevice2u)
        {
            try {
                if(has<viennamos::Device2u>()) remove<viennamos::Device2u>();
                viennamos::Device2u& device = make<viennamos::Device2u>();
                viennagrid::io::netgen_reader  reader;
                reader(device.getCellComplex(), device.getSegmentation(), filename.toStdString());
                viennagrid::scale(device.getCellComplex(), widget->getScaling());
                viennamos::copy(device, multiview);
                device_id = viennamos::Device2u::ID();
                device_segments = device.getSegmentation().size();
            }
            catch(std::exception& e) {
                QMessageBox::critical(0, QString("Error"), QString(e.what()));
            }
        }
        else
        if(type == viennamos::key::vdevice3u)
        {
            try {
                if(has<viennamos::Device3u>()) remove<viennamos::Device3u>();
                viennamos::Device3u& device = make<viennamos::Device3u>();
                viennagrid::io::netgen_reader  reader;
                reader(device.getCellComplex(), device.getSegmentation(), filename.toStdString());
                viennagrid::scale(device.getCellComplex(), widget->getScaling());
                viennamos::copy(device, multiview);
                device_id = viennamos::Device3u::ID();
                device_segments = device.getSegmentation().size();
            }
            catch(std::exception& e) {
                QMessageBox::critical(0, QString("Error"), QString(e.what()));
            }
        }
        else
        {
            QMessageBox::critical(0, QString("Error"), QString("Mesh dimension/type not yet supported!"));
            return;
        }
    }
    else
    {
        QMessageBox::critical(0, QString("Error"), "Mesh File format is not supported!");
        return;
    }


    //
    widget->setupDevice(device_segments);

    // show the loaded device in the current render window
    multiview->show_current_grid();

    multiview->resetAllViews();
}

