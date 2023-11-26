#include "hpipm-cpp/detail/d_ocp_qp_wrapper.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>


namespace hpipm {

d_ocp_qp_wrapper::d_ocp_qp_wrapper(const std::shared_ptr<d_ocp_qp_dim_wrapper>& dim) 
  : d_ocp_qp_wrapper() {
  resize(dim);
}


d_ocp_qp_wrapper::d_ocp_qp_wrapper() 
  : dim_(),
    ocp_qp_hpipm_(),
    memory_(nullptr),
    memsize_(0) {
}


d_ocp_qp_wrapper::~d_ocp_qp_wrapper() {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
}


d_ocp_qp_wrapper::d_ocp_qp_wrapper(const d_ocp_qp_wrapper& other) 
  : d_ocp_qp_wrapper() {
  copy(other);
}


d_ocp_qp_wrapper& d_ocp_qp_wrapper::operator=(const d_ocp_qp_wrapper& other) {
  if (this != &other) {
    copy(other);
  }
  return *this;
}


d_ocp_qp_wrapper::d_ocp_qp_wrapper(d_ocp_qp_wrapper&& other) noexcept
  : dim_(std::move(other.dim_)),
    ocp_qp_hpipm_(other.ocp_qp_hpipm_),
    memory_(other.memory_),
    memsize_(other.memsize_) {
  other.ocp_qp_hpipm_.dim = nullptr;
  other.ocp_qp_hpipm_.BAbt = nullptr;
  other.ocp_qp_hpipm_.RSQrq = nullptr;
  other.ocp_qp_hpipm_.DCt = nullptr;
  other.ocp_qp_hpipm_.b = nullptr;
  other.ocp_qp_hpipm_.rqz = nullptr;
  other.ocp_qp_hpipm_.d = nullptr;
  other.ocp_qp_hpipm_.d_mask = nullptr;
  other.ocp_qp_hpipm_.m = nullptr;
  other.ocp_qp_hpipm_.Z = nullptr;
  other.ocp_qp_hpipm_.idxb = nullptr;
  other.ocp_qp_hpipm_.idxs_rev = nullptr;
  other.ocp_qp_hpipm_.idxe = nullptr;
  other.ocp_qp_hpipm_.diag_H_flag = nullptr;
  other.ocp_qp_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
}


d_ocp_qp_wrapper& d_ocp_qp_wrapper::operator=(d_ocp_qp_wrapper&& other) noexcept {
  if (this == &other) return *this;

  if (memory_) {
    free(memory_);
    memory_ = nullptr;
    memsize_ = 0;
  }
  dim_ = std::move(other.dim_);
  ocp_qp_hpipm_ = other.ocp_qp_hpipm_;
  memory_ = other.memory_;
  memsize_ = other.memsize_;

  other.ocp_qp_hpipm_.dim = nullptr;
  other.ocp_qp_hpipm_.BAbt = nullptr;
  other.ocp_qp_hpipm_.RSQrq = nullptr;
  other.ocp_qp_hpipm_.DCt = nullptr;
  other.ocp_qp_hpipm_.b = nullptr;
  other.ocp_qp_hpipm_.rqz = nullptr;
  other.ocp_qp_hpipm_.d = nullptr;
  other.ocp_qp_hpipm_.d_mask = nullptr;
  other.ocp_qp_hpipm_.m = nullptr;
  other.ocp_qp_hpipm_.Z = nullptr;
  other.ocp_qp_hpipm_.idxb = nullptr;
  other.ocp_qp_hpipm_.idxs_rev = nullptr;
  other.ocp_qp_hpipm_.idxe = nullptr;
  other.ocp_qp_hpipm_.diag_H_flag = nullptr;
  other.ocp_qp_hpipm_.memsize = 0;
  other.memory_ = nullptr;
  other.memsize_ = 0;
  return *this;
}


d_ocp_qp* d_ocp_qp_wrapper::get() { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_ocp_qp_wrapper] hpipm memory is not created. Call resize() first.");
  }
  return &ocp_qp_hpipm_; 
}


const d_ocp_qp* d_ocp_qp_wrapper::get() const { 
  if (memory_ == nullptr) {
    throw std::runtime_error("[d_ocp_qp_wrapper] hpipm memory is not created. Call resize() first.");
  }
  return &ocp_qp_hpipm_; 
}


void d_ocp_qp_wrapper::resize(const std::shared_ptr<d_ocp_qp_dim_wrapper>& dim) {
  dim_ = dim;
  const hpipm_size_t new_memsize = (d_ocp_qp_memsize(dim_->get())); //don't round
  // const hpipm_size_t new_memsize = (d_ocp_qp_memsize(dim_->get())+32-1)/32*32; //round to multiples of 32
  if (memory_ != nullptr && new_memsize > memsize_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = std::max(memsize_, new_memsize);
  if (memory_ == nullptr) {
    memory_ = malloc(memsize_);
    // memory_ = aligned_alloc(32,memsize_);
  }
  d_ocp_qp_create(dim_->get(), &ocp_qp_hpipm_, memory_);
}


void d_ocp_qp_wrapper::copy(const d_ocp_qp_wrapper& other) {
  if (memory_) {
    free(memory_);
    memory_ = nullptr;
  }
  memsize_ = 0;
  resize(other.dim_);

  d_ocp_qp_copy_all(const_cast<d_ocp_qp*>(other.get()), &ocp_qp_hpipm_); 
}

} // namespace hpipm