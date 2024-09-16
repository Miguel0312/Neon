#include "accelerator.h"
#include "math/boundingBox.h"
#include "math/interval.h"
#include "scene/scene.h"
#include "scene/shape.h"

namespace Neon {
struct OctreeNode {
  union NodeData {
    struct {
      unsigned int *data;
      unsigned int cnt;
    } leaf;

    struct {
      OctreeNode **data;
    } children;
  } nodeData;

  bool isLeaf;
  BoundingBox box;

  OctreeNode() : isLeaf(false) {}

  ~OctreeNode() {
    if (isLeaf) {
      delete[] nodeData.leaf.data;
      return;
    }
    for (int i = 0; i < 8; i++) {
      if (nodeData.children.data[i] != nullptr) {
        delete nodeData.children.data[i];
      }
    }
    delete[] nodeData.children.data;
  }
};

class Octree final : public Accelerator {
public:
  Octree() = default;

  ~Octree() = default;

  void preprocess() override;

  bool rayIntersection(const Ray &r, ShapeIntersectionRecord &rec) override;

private:
  OctreeNode m_root;
  int m_maxTreeDepth = 10;
  int m_maxLeafElements = 10;

  void buildChildren(OctreeNode *parent,
                     const std::vector<unsigned int> &indices, int depth) const;

  bool rayIntersection(const Ray &r, ShapeIntersectionRecord &rec,
                       Intervalf &tInterval, OctreeNode *node);
};
} // namespace Neon
