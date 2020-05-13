#ifndef SG_VISIBLE_REACTIVE
#define SG_VISIBLE_REACTIVE

#include <src/Primitive/Visible.hpp>
// forward declaration
namespace SG::Manager{
    class Screen;
}

namespace SG::Primitive
{
    using SG::Primitive::Visible;

    class Reactive :
        virtual public Visible
    {
        friend class SG::Manager::Screen;
        public:
        bool isCurrentlyreactive() { return this->currentlyReactive; }
        virtual ~Reactive() {};
        protected:
        bool currentlyReactive = true;
        void toggleReactive()
        {
            this->currentlyReactive ? this->currentlyReactive = false : this->currentlyReactive = true;
        }
        virtual void reactiveScale(float zoomFactor) = 0;
    };
}


#endif