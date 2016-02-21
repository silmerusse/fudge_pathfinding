#ifndef FUDGE_PATHFINDING_GUI_FUDGE_INTERACTION_MANAGER_H_
#define FUDGE_PATHFINDING_GUI_FUDGE_INTERACTION_MANAGER_H_

#include "fruitcandy/engine/interaction_manager.h"
#include "fruitcandy/engine/object.h"

class FudgeInteractionManager : public InteractionManager {
public:
  FudgeInteractionManager(Realm *realm) : InteractionManager(realm) {}
  virtual ~FudgeInteractionManager() = default;

public:
  virtual void handle_mouse_button_down(unsigned int button, 
                                        int x, int y) override;
  virtual void handle_mouse_motion(int x, int y, int xrel, int yrel) override;
  virtual void handle_key_down(int keycode, bool repeat) override;

public:
  // Track selected object.
  Object *selectee_ = nullptr;

  void select(Object *selectee) {
    if (selectee_ != selectee) {
      if (selectee_)
        selectee_->selected_ = false;
      selectee->selected_ = true;
      selectee_ = selectee;
      realm_->event_target_->post_event(
          Event::create("selection_changed", selectee_->id_));
    }
  }

  void unselect() {
    if (selectee_) {
      selectee_->selected_ = false;
      realm_->event_target_->post_event(
          Event::create("selection_changed", -1));
    }
    selectee_ = nullptr;
  }

public:
  EventTarget event_target_;

private:
  // Track the mouse position.
  int x_ = 0;
  int y_ = 0;
};

#endif /* FUDGE_PATHFINDING_GUI_FUDGE_INTERACTION_MANAGER_H_ */
