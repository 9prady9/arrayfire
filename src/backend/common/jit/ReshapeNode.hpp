/*******************************************************
 * Copyright (c) 2021, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once

#include <jit/BufferNode.hpp>
#include <jit/kernel_generators.hpp>

#include <backend.hpp>
#include <iomanip>

#include <array>
#include <memory>
#include <sstream>
#include <string>

namespace common {

template<typename BufferNode>
class ReshapeNode : public Node {
   private:
    std::shared_ptr<BufferNode> mBufferNode;
    af::dim4 mViewShape;
    std::array<unsigned int, AF_MAX_DIMS> mEnd2StartDists;

   public:
    ReshapeNode(const af::dtype type, std::shared_ptr<BufferNode> bufferNode,
                const af::dim4 viewShape, const af::dim4 originalShape)
        : Node(type, 0, {}), mBufferNode(bufferNode), mEnd2StartDists(shifts) {
        static_assert(std::is_nothrow_move_assignable<ReshapeNode>::value,
                      "ReshapeNode is not move assignable");
        static_assert(std::is_nothrow_move_constructible<ReshapeNode>::value,
                      "ReshapeNode is not move constructible");
    }

    /// Default move copy constructor
    ReshapeNode(const ReshapeNode &other) = default;

    /// Default move constructor
    ReshapeNode(ReshapeNode &&other) = default;

    /// Default move/copy assignment operator(Rule of 4)
    ReshapeNode &operator=(ReshapeNode node) noexcept {
        swap(node);
        return *this;
    }

    // Swap specilization
    void swap(ReshapeNode &other) noexcept {
        using std::swap;
        Node::swap(other);
        swap(mBufferNode, other.mBufferNode);
        swap(mEnd2StartDists, other.mEnd2StartDists);
    }

    bool isLinear(dim_t dims[4]) const final {
        UNUSED(dims);
        return false;
    }

    void genKerName(std::string &kerString,
                    const common::Node_ids &ids) const final {
        kerString += '_';
        kerString += getNameStr();
        kerString += ',';
        kerString += std::to_string(ids.id);
    }

    void genParams(std::stringstream &kerStream, int id,
                   bool is_linear) const final {
        mBufferNode->genParams(kerStream, id, is_linear);
        for (int i = 0; i < 4; i++) {
            kerStream << "int shift" << id << "_" << i << ",\n";
        }
    }

    int setArgs(int start_id, bool is_linear,
                std::function<void(int id, const void *ptr, size_t arg_size)>
                    setArg) const {
        int curr_id = mBufferNode->setArgs(start_id, is_linear, setArg);
        for (int i = 0; i < 4; i++) {
            const int &d = mEnd2StartDists[i];
            setArg(curr_id + i, static_cast<const void *>(&d), sizeof(int));
        }
        return curr_id + 4;
    }

    void genOffsets(std::stringstream &kerStream, int id,
                    bool is_linear) const final {
        detail::generateShiftNodeOffsets(kerStream, id, is_linear,
                                         getTypeStr());
    }

    void genFuncs(std::stringstream &kerStream,
                  const common::Node_ids &ids) const final {
        detail::generateShiftNodeRead(kerStream, ids.id, getTypeStr());
    }

    void getInfo(unsigned &len, unsigned &buf_count,
                 unsigned &bytes) const final {
        mBufferNode->getInfo(len, buf_count, bytes);
    }

    std::string getNameStr() const final {
        return std::string("Reshape") + getShortName(m_type);
    }
};

}  // namespace common
