#include "tile_body.h"

#include "tile_collision.h"

void TileBodyInit(TileBody* body, TileBodyType type, float x, float y, float h_w, float h_h)
{
    body->position = (vec2){ x, y };
    body->half_dim = (vec2){ h_w, h_h };

    body->type = type;
    
    body->velocity = vec2_zero();
    body->sensor_offset = 0.0f;
    body->sensor_padding = 0.0f;

    TileBodyResetCollision(body);

    body->on_slope = 0;
    body->slope_detected = 0;
    body->drop = 0;

    body->gravity_scale = 1.0f;
}

void TileBodySetSensor(TileBody* body, float offset, float padding)
{
    body->sensor_offset = offset;
    body->sensor_padding = padding;
}

void TileBodyMoveX(TileBody* body, const TileMap* map, float x)
{
    vec2 old_pos = body->position;
    body->position.x += x;

    float wall_x = 0.0f;
    if (body->velocity.x <= 0.0f && TileCheckLeft(body, map, body->position, old_pos, &wall_x))
    {
        body->position.x = wall_x + body->half_dim.x;
        TileBodySetCollision(body, TILE_LEFT);
    }

    wall_x = 0.0f;
    if (body->velocity.x >= 0.0f && TileCheckRight(body, map, body->position, old_pos, &wall_x))
    {
        body->position.x = wall_x - body->half_dim.x;
        TileBodySetCollision(body, TILE_RIGHT);
    }
}

void TileBodyMoveY(TileBody* body, const TileMap* map, float y)
{
    vec2 old_pos = body->position;
    body->position.y += y;

    body->on_slope = 0;

    float ground_y = 0.0f;
    if (body->velocity.y <= 0.0f && TileCheckBottom(body, map, body->position, old_pos, &ground_y))
    {
        body->position.y = ground_y + body->half_dim.y;
        TileBodySetCollision(body, TILE_BOTTOM);
    }

    ground_y = 0.0f;
    if (body->velocity.y >= 0.0f && TileCheckTop(body, map, body->position, old_pos, &ground_y))
    {
        body->position.y = ground_y - body->half_dim.y;
        TileBodySetCollision(body, TILE_TOP);
    }
}

void TileBodyResetCollision(TileBody* body)
{
    body->collision_state[TILE_LEFT] = 0;
    body->collision_state[TILE_RIGHT] = 0;
    body->collision_state[TILE_BOTTOM] = 0;
    body->collision_state[TILE_TOP] = 0;
}

void TileBodySetCollision(TileBody* body, TileDirection dir)
{
    if (dir >= 4 || dir < 0) 
        return;
    else if (dir == TILE_LEFT || dir == TILE_RIGHT)
        body->velocity.x = 0.0f;
    else if (dir == TILE_TOP || dir == TILE_BOTTOM)
        body->velocity.y = 0.0f;

    body->collision_state[dir] = 1;
}

void TileBodyApplyGravity(TileBody* body, vec2 gravity, float slope_grip, float deltatime)
{
    float grav = body->gravity_scale * deltatime;
    if (body->on_slope && body->velocity.y <= 0.0f)
        grav *= slope_grip;

    body->velocity.x += gravity.x * grav;
    body->velocity.y += gravity.y * grav;
}

void TileBodyTick(TileBody* body, const TileMap* map, float deltatime)
{
    body->slope_detected = TileCheckSlope(body, map);

    TileBodyResetCollision(body);
    
    /* move first in x direction and then in y direction */
    TileBodyMoveX(body, map, body->velocity.x * deltatime);
    TileBodyMoveY(body, map, body->velocity.y * deltatime);

    body->drop = 0;
}

line TileBodyGetSensor(const TileBody* body, TileDirection dir, vec2 pos)
{
    line l;
    switch (dir)
    {
    case TILE_LEFT:
        l.start.x = pos.x - body->half_dim.x - body->sensor_offset;
        l.start.y = pos.y - body->half_dim.y + (body->on_slope ? 2.0f : 1.0f) * body->sensor_padding;
        l.end.x = pos.x - body->half_dim.x - body->sensor_offset;
        l.end.y = pos.y + body->half_dim.y - (body->on_slope ? 2.0f : 1.0f) * body->sensor_padding;
        break;
    case TILE_RIGHT:
        l.start.x = pos.x + body->half_dim.x + body->sensor_offset;
        l.start.y = pos.y - body->half_dim.y + (body->on_slope ? 2.0f : 1.0f) * body->sensor_padding;
        l.end.x = pos.x + body->half_dim.x + body->sensor_offset;
        l.end.y = pos.y + body->half_dim.y - (body->on_slope ? 2.0f : 1.0f) * body->sensor_padding;
        break;
    case TILE_TOP:
        l.start.x = pos.x - body->half_dim.x + body->sensor_padding;
        l.start.y = pos.y + body->half_dim.y + body->sensor_offset;
        l.end.x = pos.x + body->half_dim.x - body->sensor_padding;
        l.end.y = pos.y + body->half_dim.y + body->sensor_offset;
        break;
    case TILE_BOTTOM:
        l.start.x = pos.x - body->half_dim.x + body->sensor_padding;
        l.start.y = pos.y - body->half_dim.y - body->sensor_offset;
        l.end.x = pos.x + body->half_dim.x - body->sensor_padding;
        l.end.y = pos.y - body->half_dim.y - body->sensor_offset;
        break;
    default:
        l.start = vec2_zero();
        l.end = vec2_zero();
        break;
    }
    return l;
}