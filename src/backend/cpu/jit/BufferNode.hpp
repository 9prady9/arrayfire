/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once

#include <optypes.hpp>
#include <af/defines.h>

#include <mutex>
#include <vector>
#include "Node.hpp"
namespace cpu {

namespace jit {

using std::shared_ptr;
template<typename T>
class BufferNode : public TNode<T> {
   protected:
    shared_ptr<T> m_sptr;
    T *m_ptr;
    unsigned m_bytes;
    dim_t m_strides[AF_MAX_DIMS];
    dim_t m_dims[AF_MAX_DIMS];
    std::once_flag m_set_data_flag;
    bool m_linear_buffer;

   public:
    BufferNode() : TNode<T>(T(0), 0, {}) {}

    void setData(shared_ptr<T> data, unsigned bytes, dim_t data_off,
                 const dim_t *dims, const dim_t *strides,
                 const bool is_linear) {
        std::call_once(m_set_data_flag, [this, data, bytes, data_off, dims,
                                         strides, is_linear]() {
            m_sptr          = data;
            m_ptr           = data.get() + data_off;
            m_bytes         = bytes;
            m_linear_buffer = is_linear;
            for (int i = 0; i < 4; i++) {
                m_strides[i] = strides[i];
                m_dims[i]    = dims[i];
            }
        });
    }

    void calc(int x, int y, int z, int w, int lim) final {
        using Tc = compute_t<T>;

        dim_t l_off = 0;
        l_off += (w < (int)m_dims[3]) * w * m_strides[3];
        l_off += (z < (int)m_dims[2]) * z * m_strides[2];
        l_off += (y < (int)m_dims[1]) * y * m_strides[1];
        T *in_ptr   = m_ptr + l_off;
        Tc *out_ptr = this->m_val.data();
        for (int i = 0; i < lim; i++) {
            out_ptr[i] =
                static_cast<Tc>(in_ptr[((x + i) < m_dims[0]) ? (x + i) : 0]);
        }
    }

    void calc(int idx, int lim) final {
        using Tc = compute_t<T>;

        T *in_ptr   = m_ptr + idx;
        Tc *out_ptr = this->m_val.data();
        for (int i = 0; i < lim; i++) {
            out_ptr[i] = static_cast<Tc>(in_ptr[i]);
        }
    }

    void getInfo(unsigned &len, unsigned &buf_count,
                 unsigned &bytes) const final {
        len++;
        buf_count++;
        bytes += m_bytes;
        return;
    }

    size_t getBytes() const final { return m_bytes; }

    void genKerName(std::string &kerString,
                    const common::Node_ids &ids) const final {
        UNUSED(kerString);
        UNUSED(ids);
    }

    void genParams(std::stringstream &kerStream, int id,
                   bool is_linear) const final {
        UNUSED(kerStream);
        UNUSED(id);
        UNUSED(is_linear);
    }

    int setArgs(int start_id, bool is_linear,
                std::function<void(int id, const void *ptr, size_t arg_size)>
                    setArg) const override {
        UNUSED(is_linear);
        UNUSED(setArg);
        return start_id++;
    }

    void genOffsets(std::stringstream &kerStream, int id,
                    bool is_linear) const final {
        UNUSED(kerStream);
        UNUSED(id);
        UNUSED(is_linear);
    }

    void genFuncs(std::stringstream &kerStream,
                  const common::Node_ids &ids) const final {
        UNUSED(kerStream);
        UNUSED(ids);
    }

    bool isLinear(dim_t *dims) const final {
        size_t selems  = 1;
        size_t delems  = 1;
        bool sameShape = true;
        for (int d = 0; d < AF_MAX_DIMS; ++d) {
            selems *= m_dims[d];
            delems *= dims[d];
            sameShape &= (dims[d] == m_dims[d]);
        }
        const bool sameTotalElems = selems == delems;
        // If same shape or total number of elements for the  shapes
        // pointed by argument 'dim' and member variable 'm_param.dims'
        // then return m_linear_buffer flag.
        // Otherwise, return false.
        return (sameShape || sameTotalElems ? m_linear_buffer : false);
    }

    bool isBuffer() const final { return true; }
};

}  // namespace jit
}  // namespace cpu
