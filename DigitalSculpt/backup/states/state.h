#include "scene.h"

namespace DigitalSculpt
{
    class State
    {
    protected:
        SceneSPtr _main;
    public:
        State(SceneSPtr main) :_main(main)
        {
        }

        virtual ~State()
        {
            _main.reset();
        }

        virtual bool isNoop() = 0;

        virtual void undo() = 0;

        virtual void redo() = 0;

        virtual std::shared_ptr<State> createRedo() = 0;
    };

    typedef std::shared_ptr<State> StateSPtr;
}