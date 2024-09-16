#include "scene/accelerators/octree.h"
#include <iostream>

namespace Neon {
void Octree::preprocess() {
  if (m_scene == nullptr) {
    std::cout << "ERROR: accelerator's scene is not set" << std::endl;
    return;
  }
  m_root.box = m_scene->getBoundingBox();
  std::vector<unsigned int> indices(m_scene->getShapes().size());
  for (int i = m_scene->getShapes().size() - 1; i >= 0; i--) {
    indices[i] = i;
  }
  buildChildren(&m_root, indices, 0);
}

void Octree::buildChildren(OctreeNode *parent,
                           const std::vector<unsigned int> &indices,
                           int depth) const {
  parent->nodeData.children.data = new OctreeNode *[8];
  for (int i = 0; i < 8; i++) {
    parent->nodeData.children.data[i] = nullptr;
  }

  std::vector<unsigned int> childIndices;
  unsigned int cnt = indices.size();

  for (int i = 0; i < 8; i++) {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    minX = (i & 1) ? parent->box.getMin().x() : parent->box.getCenter().x();
    minY = (i & 2) ? parent->box.getMin().y() : parent->box.getCenter().y();
    minZ = (i & 4) ? parent->box.getMin().z() : parent->box.getCenter().z();

    maxX = (i & 1) ? parent->box.getCenter().x() : parent->box.getMax().x();
    maxY = (i & 2) ? parent->box.getCenter().y() : parent->box.getMax().y();
    maxZ = (i & 4) ? parent->box.getCenter().z() : parent->box.getMax().z();

    BoundingBox newBox(Point3f(minX, minY, minZ), Point3f(maxX, maxY, maxZ));

    childIndices.clear();
    for (unsigned int i = 0; i < cnt; i++) {
      int x = indices[i];
      if (m_scene->getShapes()[x]->getBoundingBox().intersects(newBox)) {
        childIndices.push_back(x);
      }
    }

    if (childIndices.empty()) {
      continue;
    }

    OctreeNode *childNode = new OctreeNode();
    childNode->box = newBox;

    if (childIndices.size() <= (size_t)m_maxLeafElements ||
        depth >= m_maxTreeDepth) {
      childNode->isLeaf = true;
      childNode->nodeData.leaf.cnt = childIndices.size();
      childNode->nodeData.leaf.data = new unsigned int[childIndices.size()];
      std::copy(childIndices.begin(), childIndices.end(),
                childNode->nodeData.leaf.data);
    } else {
      childNode->isLeaf = false;
      buildChildren(childNode, childIndices, depth + 1);
    }

    parent->nodeData.children.data[i] = childNode;
  }
}

bool Octree::rayIntersection(const Ray &r, ShapeIntersectionRecord &rec) {
  Intervalf tInterval(1e-5, 100);
  return rayIntersection(r, rec, tInterval, &m_root);
}

bool Octree::rayIntersection(const Ray &r, ShapeIntersectionRecord &rec,
                             Intervalf &tInterval, OctreeNode *node) {
  bool result = false;
  if (node->isLeaf) {
    for (unsigned int i = 0; i < node->nodeData.leaf.cnt; i++) {
      if (m_scene->getShapes()[node->nodeData.leaf.data[i]]->intersect(
              r, tInterval, rec)) {
        tInterval.setMax(rec.t);
        result = true;
      }
    }

    return result;
  }

  for (int i = 0; i < 8; i++) {
    OctreeNode *child = node->nodeData.children.data[i];
    if (child == nullptr || !child->box.intersects(r, tInterval))
      continue;
    result |= rayIntersection(r, rec, tInterval, child);
  }

  return result;
}
} // namespace Neon
