#include "camera.h"
#include "sprite.h"
#include "renderable.h"
#include "object.h"

Camera::Camera(const SDL_Rect& rect, Realm *realm,
               const std::vector<std::string> &store_names) :
    rect_(rect), realm_(realm), store_names_(store_names) {
}

void Camera::shoot() {
  renderables_.clear();
  for (auto store_name : store_names_) {
    realm_->object_manager_->traverse_store<Object>(
        store_name, [&](Object *object) {
          for (auto &component: object->components_) {
            if (dynamic_cast<Renderable*>(component.get())) {
              Renderable *r = dynamic_cast<Renderable*>(component.get());
              auto rect = r->get_rect();
              if (SDL_HasIntersection(&rect, &rect_)) {
                renderables_.push_back(r);
              }
            }
          }
        });
  }
}
