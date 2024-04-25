#include "states/state_custom.h"

namespace DigitalSculpt
{
    StateCustom::StateCustom(std::function<void(void)> undocb, std::function<void(void)> redocb) :State(std::make_shared<Scene>(nullptr))
    {
        _undocb = undocb;
        _redocb = redocb ? redocb : undocb;
    }

    StateCustom::~StateCustom()
    {
    }

    bool StateCustom::isNoop()
    {
        return _undocb.empty();
    }

    void StateCustom::undo()
    {
        if (!_undocb)
            _undocb();
    }

    void StateCustom::redo()
    {
        if (!_redocb)
            _redocb();
    }

    std::shared_ptr<State> StateCustom::createRedo()
    {
        return std::make_shared<StateCustom>(new StateCustom(_undocb, _redocb));
    }
}