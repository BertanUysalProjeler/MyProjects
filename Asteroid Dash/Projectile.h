#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <vector>

class Projectile {
public:
    std::vector<std::vector<bool>> shape = { {1} };

    int position_row;
    int position_column;
    int appearanceTime;

    Projectile* nextProjectile = nullptr;

    Projectile(int position_row, int position_column, int appearanceTime);

    ~Projectile();
};

#endif // PROJECTILE_H
