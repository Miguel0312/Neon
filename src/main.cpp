#include "scene/accelerators/octree.h"
#include "scene/sceneParser.h"
#include "thirdparty/argparse/argparse.h"
#include <math/vector.h>
#include <utils/color.h>
#include <utils/image.h>

#define WIDTH 800
#define HEIGHT 600

using namespace Neon;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser argParser("Neon", "0.1.0");

  argParser.add_argument("sceneFile");
  argParser.add_argument("-j").scan<'i', int>();

  argParser.parse_args(argc, argv);

  SceneParser sceneParser(argParser.get("sceneFile"), argParser);

  sceneParser.renderScene();

  return 0;
}
