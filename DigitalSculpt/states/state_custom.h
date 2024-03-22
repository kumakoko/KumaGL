#include <vector>
#include <functional>
#include <memory>
#include "states/state.h"

namespace DigitalSculpt
{
    class StateCustom : public State
    {
    private:
        std::function<void(void)> _undocb;
        std::function<void(void)> _redocb;
    public:
        StateCustom(std::function<void(void)> undocb, std::function<void(void)> redocb);

        virtual ~StateCustom();

        virtual bool isNoop() override;

        virtual void undo() override;

        virtual void redo() override;

        virtual std::shared_ptr<State> createRedo() override;
    };
}