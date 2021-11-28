#include "BoardMap.h"

BoardMap::BoardMap(const int width, const int height, const int qtyBombs):
  width(width),
  height(height),
  qtyBombs(qtyBombs),
  mines(new Mine*[height]),
  firstMineClicked(nullptr) {

  for (auto i = 0; i < height; i++) {
    mines[i] = new Mine[width];
  }
}


void BoardMap::drawMap(Node* parent) const {
  for (auto y = 0; y < height; ++y) {
    for (auto x = 0; x < width; ++x) {
      mines[y][x].initialize(x, y);

      const auto posX = (x + 1) * mines[y][x].getContentSize().width / 2;
      const auto posY = (y + 1) * mines[y][x].getContentSize().height / 2;

      mines[y][x].setPosition(posX, posY);
      mines[y][x].setScale(0.5);

      parent->addChild(&mines[y][x]);
    }
  }
}

void BoardMap::initializeMines(const Vec2 firstClickPosition) {
  this->firstMineClicked = getMine(firstClickPosition);
  hasInitialized = true;

  createBombMines();
  createCounterNearMines();
}

void BoardMap::createBombMines() const {
  auto qtyBombsToDistribute = qtyBombs;

  while (qtyBombsToDistribute) {
    auto randomPosition = getRandomMinePosition();
    auto isValidPlace = canPlaceBomb(randomPosition.x, randomPosition.y);

    auto mineBomb = &mines[randomPosition.y][randomPosition.x];

    while (mineBomb->mineType == bomb || !isValidPlace) {
      randomPosition = getRandomMinePosition();
      mineBomb = &mines[randomPosition.y][randomPosition.x];

      isValidPlace = canPlaceBomb(randomPosition.x, randomPosition.y);
    }

    mineBomb->mineType = bomb;
    --qtyBombsToDistribute;
  }
}

void BoardMap::createCounterNearMines() const {
  for (auto y = 0; y < height; ++y) {
    for (auto x = 0; x < width; ++x) {
      const auto mine = &mines[y][x];
      
      if (mine->mineType != none) {
        continue;
      }

      if (x != 0) {
        if (y != 0 && mines[y - 1][x - 1].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }

        if (mines[y][x - 1].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }

        if (y != height - 1 && mines[y + 1][x - 1].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }
      }

      if (x != width - 1) {
        if (y != 0 && mines[y - 1][x + 1].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }

        if (mines[y][x + 1].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }

        if (y != height - 1 && mines[y + 1][x + 1].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }
      }
      
      if (y != 0) {
        if (mines[y - 1][x].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }
      }

      if (y != height - 1) {
        if (mines[y + 1][x].mineType == bomb) {
          mine->setNearBombCount(mine->getNearBombCount() + 1);
        }
      }
    }
  }
}


void BoardMap::openAdjacentMinesAt(const Mine& mine) const {
  auto const x = mine.x;
  auto const y = mine.y;

  mines[y][x].dig();

  if (mines[y][x].mineType == counter) {
    return;
  }

  if (y > 0 && !mines[y - 1][x].wasDug)
    openAdjacentMinesAt(mines[y - 1][x]);
  if (x > 0 && !mines[y][x - 1].wasDug)
    openAdjacentMinesAt(mines[y][x - 1]);
  if (x < width - 1 && !mines[y][x + 1].wasDug)
    openAdjacentMinesAt(mines[y][x + 1]);
  if (y < height - 1 && !mines[y + 1][x].wasDug)
    openAdjacentMinesAt(mines[y + 1][x]);
}


void BoardMap::onClick(const Vec2 location) {
  auto const mine = getMine(location);
  if (mine == nullptr) {
    return;
  }

  if (!hasInitialized) {
    initializeMines(location);
  }

  mine->dig();

  if (mine->mineType == none) {
    openAdjacentMinesAt(*mine);
  }
}


Mine* BoardMap::getMine(const Vec2 location) const {
  for (auto y = 0; y < height; ++y) {
    for (auto x = 0; x < width; ++x) {
      const auto mine = &mines[y][x];
      const auto isMinePoint = mine->getBoundingBox().containsPoint(location);

      if (isMinePoint) {
        return mine;
      }
    }
  }

  return nullptr;
}

MinePosition BoardMap::getRandomMinePosition() const {
  const uniform_int_distribution<> widthRandomizer(0, width - 1);
  const uniform_int_distribution<> heightRandomizer(0, height - 1);

  random_device randomizer;
  mt19937 mtGenerator(randomizer());

  const auto x = widthRandomizer(mtGenerator);
  const auto y = heightRandomizer(mtGenerator);

  return {x, y};
}

bool BoardMap::canPlaceBomb(const int x, const int y) const {
  const auto mine = this->firstMineClicked;

  // Center
  auto isInSquareRange = x == mine->x && y == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  //Right
  isInSquareRange = x + 1 == mine->x && y == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  //Left
  isInSquareRange = x - 1 == mine->x && y == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  // Top
  isInSquareRange = x == mine->x && y + 1 == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  // Bottom
  isInSquareRange = x == mine->x && y - 1 == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  // Top-Right
  isInSquareRange = x + 1 == mine->x && y + 1 == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  // Top-Left
  isInSquareRange = x - 1 == mine->x && y + 1 == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  // Bottom-Right
  isInSquareRange = x + 1 == mine->x && y - 1 == mine->y;
  if (isInSquareRange) {
    return !isInSquareRange;
  }

  // Bottom-Left
  isInSquareRange = x - 1 == mine->x && y - 1 == mine->y;
  return !isInSquareRange;
}
