#ifndef BULLET_CONTROLLER_H
#define BULLET_CONTROLLER_H

#include "component.h"
#include "input_locator.h"

namespace galaga {
    class BulletController : public nim::Component {
    public:
        BulletController();
        BulletController(const BulletController &other);
        BulletController(BulletController &&other);
        ~BulletController();

        BulletController &operator=(const BulletController &other);
        BulletController &operator=(BulletController &&other);

        virtual void Update() override;

        void Fire();

    private:
        nim::Input *input;// owned by input locator;
    };
}// namespace galaga
#endif//BULLET_CONTROLLER_H