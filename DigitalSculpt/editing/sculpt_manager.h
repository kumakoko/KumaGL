#ifndef digital_sculpt_sculpt_manager_h__
#define digital_sculpt_sculpt_manager_h__

#include <vector>
#include <cstdint>
#include "drawables/selection.h"
#include "editing/tools/tools.h"
#include "editing/tools/sculpt_base.h"
#include "misc/enums.h"

namespace DigitalSculpt
{
    class Scene;

    class SculptManager
    {
    private:
        Scene* _main;
        Enums::Tools _toolIndex;
        std::vector<SculptBase*> _tools;
        bool _symmetry;
        bool _continuous;
        std::int32_t _sculptTimer;
        Selection* _selection;
    public:
        SculptManager(Scene* main);

        inline void setToolIndex(Enums::Tools id)
        {
            this->_toolIndex = id;
        }

        inline Enums::Tools getToolIndex()
        {
            return this->_toolIndex;
        }

        inline SculptBase* getCurrentTool()
        {
            return this->_tools[this->_toolIndex];
        }

        inline bool getSymmetry() const
        {
            return this->_symmetry;
        }

        inline SculptBase* getTool(Enums::Tools index)
        {
            return this->_tools[index];
        }

        inline const SculptBase* getTool(Enums::Tools index) const
        {
            return this->_tools[index];
        }

        inline Selection* getSelection()
        {
            return this->_selection;
        }

        inline const Selection* getSelection() const
        {
            return this->_selection;
        }

        void init();

        bool canBeContinuous();

        inline bool isUsingContinuous()
        {
            return this->_continuous && this->canBeContinuous();
        }

        bool start(bool ctrl);

        void end();

        void preUpdate();

        void update();

        void postRender();

        void addSculptToScene(Scene* scene);
    };

}
#endif // digital_sculpt_sculpt_manager_h__