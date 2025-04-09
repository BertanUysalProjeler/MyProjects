#include "Projectile.h"

Projectile::Projectile(int position_row, int position_column, int appearanceTime)
    :position_row(position_row), position_column(position_column), appearanceTime(appearanceTime) {}

Projectile::~Projectile() {
    // Delete the nextProjectile if it exists
    if (nextProjectile != nullptr) {
        delete nextProjectile;
        nextProjectile = nullptr; // Set to nullptr to avoid a dangling pointer
    }
}
