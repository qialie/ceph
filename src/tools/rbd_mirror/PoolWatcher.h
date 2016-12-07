// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_RBD_MIRROR_POOL_WATCHER_H
#define CEPH_RBD_MIRROR_POOL_WATCHER_H

#include <map>
#include <memory>
#include <set>
#include <string>

#include "common/ceph_context.h"
#include "common/Mutex.h"
#include "common/Timer.h"
#include "include/rados/librados.hpp"
#include "types.h"
#include <boost/optional.hpp>

namespace rbd {
namespace mirror {

/**
 * Keeps track of images that have mirroring enabled within all
 * pools.
 */
class PoolWatcher {
public:
  PoolWatcher(librados::IoCtx &remote_io_ctx, double interval_seconds,
	      Mutex &lock, Cond &cond);
  ~PoolWatcher();
  PoolWatcher(const PoolWatcher&) = delete;
  PoolWatcher& operator=(const PoolWatcher&) = delete;

  bool is_blacklisted() const;

  const ImageIds& get_images() const;
  void refresh_images(bool reschedule=true);

private:
  librados::IoCtx m_remote_io_ctx;
  Mutex &m_lock;
  Cond &m_refresh_cond;

  bool m_stopping = false;
  bool m_blacklisted = false;
  SafeTimer m_timer;
  double m_interval;

  ImageIds m_images;

  int refresh(ImageIds *image_ids);
};

} // namespace mirror
} // namespace rbd

#endif // CEPH_RBD_MIRROR_POOL_WATCHER_H
