#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

extern const struct wl_interface tizen_rotation_interface;
extern const struct wl_interface wl_surface_interface;

static const struct wl_interface *types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&tizen_rotation_interface,
	&wl_surface_interface,
	&wl_surface_interface,
};

static const struct wl_message tizen_policy_ext_requests[] = {
	{ "get_rotation", "no", types + 5 },
	{ "get_active_angle", "?o", types + 7 },
};

static const struct wl_message tizen_policy_ext_events[] = {
	{ "active_angle", "u", types + 0 },
};

WL_EXPORT const struct wl_interface tizen_policy_ext_interface = {
	"tizen_policy_ext", 3,
	2, tizen_policy_ext_requests,
	1, tizen_policy_ext_events,
};

static const struct wl_message tizen_rotation_requests[] = {
	{ "destroy", "", types + 0 },
	{ "set_available_angles", "u", types + 0 },
	{ "set_preferred_angle", "u", types + 0 },
	{ "ack_angle_change", "u", types + 0 },
	{ "set_geometry_hint", "3uuuuu", types + 0 },
};

static const struct wl_message tizen_rotation_events[] = {
	{ "available_angles_done", "u", types + 0 },
	{ "preferred_angle_done", "u", types + 0 },
	{ "angle_change", "uu", types + 0 },
	{ "angle_change_with_resize", "2uuuu", types + 0 },
};

WL_EXPORT const struct wl_interface tizen_rotation_interface = {
	"tizen_rotation", 3,
	5, tizen_rotation_requests,
	4, tizen_rotation_events,
};

