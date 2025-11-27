[![Release](https://img.shields.io/badge/Release-v2.0.0-ffb600.svg?style=for-the-badge)](https://github.com/Santiagodiazs/TLA_Plotter/releases)
[![Build Status](https://github.com/Santiagodiazs/TLA_Plotter/actions/workflows/pipeline.yaml/badge.svg?branch=development)](https://github.com/Santiagodiazs/TLA_Plotter/actions/workflows/pipeline.yaml)

# 🎨 TLA Plotter

A powerful Domain-Specific Language (DSL) compiler that transforms declarative scene descriptions into beautiful SVG graphics. Write simple, intuitive code and generate production-ready vector graphics.

* [Requirements](#requirements)
* [Configuration](#configuration)
* [Commands](#commands)
* [Features](#features)
* [Language Syntax](#language-syntax)
* [CI/CD](#cicd)
* [Recommended Extensions](#recommended-extensions)

```plot
scene MyScene {
    draw circle sun {
        at (400, 100);
        radius 50;
        fill yellow;
    }
    
    draw rectangle house {
        at (300, 300);
        size 200x150;
        fill #8B4513;
    }
}
```

## Requirements

* [Docker v28.3.2](https://www.docker.com/)

## Configuration

Set the following environment variables to control and configure the behaviour of the application:

| Name                  | Default | Description                                                                                                                                                           |
| :-------------------- | :-----: | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ENVIRONMENT`         | `Local` | The active environment name. The available environments are: `Local`, `Development` and `Production`.                                                                 |
| `LOG_IGNORED_LEXEMES` | `true`  | When `true`, logs all of the ignored lexemes found with Flex at `DEBUGGING` level. To remove those logs from the console output set it to `false`.                    |
| `LOGGING_LEVEL`       | `ALL`   | The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`. |
| `YYDEBUG`             | `false` | When `true`, enables Bison debug mode for detailed parsing traces.                                                                                                    |

_Docker Compose_ can read the variables from an `.env` file too (see `compose.yaml` file).

## Commands

### Start

Rises an ephemeral container, ready to start development:

```bash
docker compose run --rm compiler
```

### Build

Builds or rebuilds the entire compiler:

```bash
src/main/bash/build.sh
```

### Run

Compiles a program:

```bash
src/main/bash/run.sh <program>
```

where `<program>` is the path to the file that represents its entry-point.

### Test

Executes every available unit-test under `src/test/c` folder:

```bash
src/main/bash/test.sh
```

### Stop

Logout, destroy the ephemeral containers and shutdowns the cluster:

```bash
exit
docker compose down
```

### Docker

| Command                                 | Description                                             |
| :-------------------------------------- | :------------------------------------------------------ |
| `docker builder prune --all`            | Removes all builds and complete build cache.            |
| `docker compose --progress=plain build` | Forces a build or rebuild of the images in the cluster. |
| `docker image prune`                    | Removes all of the dangling images from Docker.         |
| `docker network prune`                  | Removes unused networks from Docker.                    |
| `docker volume prune`                   | Removes unused volumes from Docker.                     |

## ✨ Features

### 🎯 Core Graphics
- **Primitive Shapes**: Rectangles, circles, ellipses, and lines
- **Flexible Sizing**: Use `size`, `width`/`height`, or `radius` with full unit support
- **Rich Colors**: Named colors, hex (`#FF0000`), RGB (`rgb(255,0,0)`), and RGBA (`rgba(255,0,0,0.5)`)

### 🎨 Advanced Styling
- **Color Palettes**: Define reusable color schemes and reference them across your scene
- **Units**: Support for `px`, `rem`, `em`, `vw`, `vh`, and `%`
- **Opacity & Strokes**: Fine-grained control over fill, stroke, and transparency

### 🔧 Smart Features
- **Variables**: Define and reuse values across your scene
  ```plot
  baseSize = 100;
  offset = 50;
  width baseSize px;      // Uses variable value
  height baseSize + offset px;  // Combine with expressions
  ```
- **Math Expressions**: Dynamic values with compile-time evaluation
  ```plot
  width 100 + 50 px;      // Evaluates to 150px
  radius (10 + 5) * 2;    // Evaluates to 30
  ```
- **Transformations**: Translate, rotate, and scale with ease
- **Layers & Symbols**: Organize complex scenes with reusable components
- **Z-index Control**: Layer management for overlapping elements

### 🛡️ Robust Validation
- **Semantic Analysis**: Catches invalid property combinations and missing requirements
- **Type Safety**: Ensures properties match their respective figure types
- **Conflict Detection**: Prevents duplicate or contradictory declarations

## 📖 Language Syntax

### Basic Scene
```plot
scene SceneName {
    draw rectangle myRect {
        at (x, y);
        size 100x50;
        fill red;
    }
}
```

### Color Palettes
```plot
scene StyledScene {
    palette theme {
        primary #3498db;
        accent rgb(231, 76, 60);
        transparent rgba(52, 152, 219, 0.5);
    }
    
    draw circle c {
        at (100, 100);
        radius 50;
        fill theme.primary;
    }
}
```

### Transformations
```plot
draw rectangle box {
    at (100, 100);
    size 50x50;
    fill blue;
    translate (20, 30);
    rotate 45;
    scale 1.5;
}
```

### Math Expressions
```plot
draw rectangle dynamic {
    at (0, 0);
    width (100 + 50) * 2 px;        // 300px
    height 20 * (5 + 3) px;          // 160px
    fill green;
}
```

### Units
```plot
draw rectangle responsive {
    at (10, 10);
    width 50 vw;      // 50% of viewport width
    height 30 vh;     // 30% of viewport height
    fill purple;
}
```

### Variables
```plot
scene VariableDemo {
    baseSize = 100;
    spacing = 20;
    scaleFactor = 1.5;
    
    draw rectangle box1 {
        at (spacing, spacing);
        width baseSize px;
        height baseSize * scaleFactor px;
        fill blue;
    }
    
    draw circle c {
        at (baseSize + spacing * 2, spacing);
        radius baseSize / 2;
        fill red;
    }
}
```

## 🧪 Testing

Run the complete test suite:
```bash
src/main/bash/test.sh
```

The test suite includes:
- ✅ **18 acceptance tests** - Valid scenes that should compile
- ❌ **16 rejection tests** - Invalid scenes that should fail validation

## CI/CD

To trigger an automatic integration on every push or PR (_Pull Request_), you must activate _GitHub Actions_ in the _Settings_ tab. Use the following configuration:

| Key                                                        | Value                                               |
| :--------------------------------------------------------- | :-------------------------------------------------- |
| `Actions permissions`                                      | `Allow all actions and reusable workflows`          |
| `Allow GitHub Actions to create and approve pull requests` | `false`                                             |
| `Artifact and log retention`                               | `30 days`                                           |
| `Fork pull request workflows from outside collaborators`   | `Require approval for all outside collaborators`    |
| `Workflow permissions`                                     | `Read repository contents and packages permissions` |

## Recommended Extensions

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)

---

<p align="center">Made with ❤️ for vector graphics enthusiasts</p>
