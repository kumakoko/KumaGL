#include "scene.h"
#include "editing/sculpt_manager.h"

namespace DigitalSculpt
{
    SculptManager::SculptManager(Scene* main)
    {
        this->_main = main;

        this->_toolIndex = Enums::Tools::BRUSH; // sculpting mode
        this->_tools = []; // the sculpting tools

        // symmetry stuffs
        this->_symmetry = true; // if symmetric sculpting is enabled  

        // continuous stuffs
        this->_continuous = false; // continuous sculpting
        this->_sculptTimer = -1; // continuous interval timer

        this->_selection = new Selection(); // the selection geometry (red hover circle)

        this->init();
    }

    void SculptManager::init() 
    {
        /* var main = this->_main;
         var tools = this->_tools;
         for (var i = 0, nb = _tools.size(); i < nb; ++i) {
             if (Tools[i]) tools[i] = new Tools[i](main);
         }*/
    }

    bool SculptManager::canBeContinuous() 
    {
        switch (this->_toolIndex)
        {
        case Enums::Tools::TWIST:
        case Enums::Tools::MOVE:
        case Enums::Tools::DRAG:
        case Enums::Tools::LOCALSCALE:
        case Enums::Tools::TRANSFORM:
            return false;
        default:
            return true;
        }
    }

    bool SculptManager::isUsingContinuous()
    {
        return _continuous && canBeContinuous();
    }

    bool SculptManager::start(bool ctrl) 
    {
        SculptBase* tool = getCurrentTool();
        bool canEdit = tool->start(ctrl);

        if (_main->getPicking()->getMesh() != nullptr && isUsingContinuous())
            _sculptTimer = window.setInterval(tool._cbContinuous, 16.6);
        
        return canEdit;
    }

    void SculptManager::end()
    {
        getCurrentTool()->end();
        
        if (_sculptTimer != = -1)
        {
            clearInterval(_sculptTimer);
            _sculptTimer = -1;
        }
    }

    void SculptManager::preUpdate() 
    {
        getCurrentTool()->preUpdate(canBeContinuous());
    }

    void SculptManager::update()
    {
        if (isUsingContinuous())
            return;

        getCurrentTool()->update();
    }

    void SculptManager::postRender()
    {
        getCurrentTool()->postRender(_selection);
    }

    void SculptManager::addSculptToScene(Scene* scene)
    {
        return getCurrentTool()->addSculptToScene(scene);
    }
}