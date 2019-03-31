#pragma once

#include <mbed.h>

template<typename T>
class ControlLoop
{
protected:
  float m_dt;
  T m_desired;
  Thread m_thread;
  Mutex m_mutex;

public:
  ControlLoop(float dt):
  m_dt(dt)
  {}

  virtual ~ControlLoop(){}

  virtual void start()
  {
    m_thread.start(callback(this, &ControlLoop::loop));
  }

  //Thread-safe
  void set_desired(T desired)
  {
    // This needs to grab a mutex to be thread safe.
    m_mutex.lock();
    m_desired = desired;
    m_mutex.unlock();
  }

  T get_desired()
  {
    m_mutex.lock();
    T desired(m_desired); // make a copy
    m_mutex.unlock();
    return desired;
  }

protected:
  virtual void update() = 0;

private:
  void loop()
  {
    while(true){
      update();
      wait(m_dt);
    }
  }
};
