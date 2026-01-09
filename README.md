# Space Collector

A fast-paced arcade shooter game built with **raylib** where you pilot a spaceship to collect coins and destroy incoming asteroids.

## 🎮 Game Overview

Space Collector is a 2D arcade game combining resource collection and action combat. Navigate your spaceship through an endless stream of falling asteroids, collect valuable coins for points, and use your weapons to defend against threats.

## 🎯 Gameplay Features

### Controls
- **W** - Move Up
- **A** - Move Left
- **S** - Move Down
- **D** - Move Right
- **ENTER** - Start Game / Restart After Game Over

### Mechanics
- **Coin Collection**: Collect coins scattered across the screen for 100 points each
- **Asteroid Destruction**: Shoot falling asteroids to destroy them and earn 50 points
- **Particle Effects**: Visual feedback with particle explosions on hits
- **High Score Tracking**: Your best score is saved and persists between sessions
- **Screen Shake**: Camera shakes on collision for impact feedback

### Game States
1. **Title Screen** - Shows the game title and highest score
2. **Gameplay** - Active game with enemies, coins, and scoring
3. **Game Over** - Displays final and highest scores; restart available

## 🛠️ Technical Details

### Built With
- **raylib** - Cross-platform graphics and audio library
- **C++** - Core game logic
- **Windows Resource File** (resource.rc) - Game resources and metadata

### Project Structure
```
Space-Collector/
├── Space Collector.cpp    # Main game source code
├── resource.rc            # Windows resource configuration
├── README.md             # This file
└── assets/
    ├── storage.bat       # High score persistence file
    ├── icons/            # Icon assets
    ├── Images/
    │   ├── spaceship.png # Player sprite
    │   ├── Rock.png      # Enemy asteroid sprite
    │   └── Space.png     # Background image
    └── Sound/
        ├── BGM.mp3       # Background music
        ├── Explosion.mp3 # Impact sound effect
        └── coin.mp3      # Coin collection sound
```

### Game Systems
- **Collision Detection**: Rectangle-based collision for player-coin, player-enemy, and bullet-enemy
- **Particle System**: 100 particle limit with alpha fade animations
- **Audio Management**: Background music loop with sound effects
- **Score Persistence**: High score saved to `storage.bat`

## 🚀 Getting Started

### Requirements
- C++ compiler (MSVC, GCC, or Clang)
- raylib library installed
- Windows OS (due to resource.rc and hardcoded paths)

### Building
```bash
g++ "Space Collector.cpp" resource.o -lraylib -lopengl32 -lgdi32 -lwinmm -o "Space Collector.exe"
```

### Running
Simply execute the compiled executable:
```bash
"Space Collector.exe"
```

## 📊 Scoring System

| Action | Points |
|--------|--------|
| Collect Coin | 100 |
| Destroy Asteroid | 50 |

Your highest score is automatically saved when you lose and will display on the title screen.

## 🎨 Visual Style

- **Classic arcade aesthetic** with a scrolling space background
- **Colorful UI elements**: Yellow bullets, Yellow coins, Red text for warnings
- **Smooth animations**: 60 FPS gameplay with particle effects
- **Screen shake feedback** on player collision

## 🔧 Future Enhancement Ideas

- [ ] Difficulty progression (increasing enemy speed/spawn rate)
- [ ] Power-ups system (shields, rapid-fire, etc.)
- [ ] Enemy variety with different behaviors
- [ ] Leaderboard system
- [ ] Mobile/web port using raylib's emscripten support
- [ ] Sound volume controls
- [ ] Pause functionality
