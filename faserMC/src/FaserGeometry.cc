#include "FaserGeometry.hh"

#include "G4UImanager.hh"
#include "G4UIcommandTree.hh"
#include "G4UIcommand.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


//------------------------------------------------------------------------------

FaserGeometry::FaserGeometry() { }

FaserGeometry::FaserGeometry(G4String path)
{
    if (path == "") return;
    G4UImanager* manager = G4UImanager::GetUIpointer();
    G4UIcommandTree* tree = manager->GetTree();
    G4UIcommandTree* faser = tree->GetTree(path);
    NavigateCommandTree(faser->GetPathName(), faser);
}

void FaserGeometry::NavigateCommandTree(G4String path, G4UIcommandTree* tree)
{
    if (tree == nullptr) return;
    for (int i = 1; i <= tree->GetCommandEntry(); i++)
    {
        G4UIcommand* cmd = tree->GetCommand(i);
        if (cmd->GetCurrentValue().strip(G4String::both).length() > 0)
        {
            fProperties[path][cmd->GetCommandName()] = cmd->GetCurrentValue();
            //G4cout << "Path: " << path << " Name: " << cmd->GetCommandName() << ": " << cmd->GetCurrentValue() << G4endl;   
        }
    }

    for (int i = 1; i <= tree->GetTreeEntry(); i++)
    {
        G4UIcommandTree* subTree = tree->GetTree(i);
        NavigateCommandTree(subTree->GetPathName(), subTree);
    }
}

FaserGeometry::~FaserGeometry() { }

