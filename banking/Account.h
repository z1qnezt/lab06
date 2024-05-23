#pragma once

class Account {
 public:
  Account(int id, int balance);
  virtual ~Account();

  // Virtual to test.
  virtual int GetBalance() const;

  // Virtual to test.
  virtual void ChangeBalance(int diff);

  // Virtual to test.
  virtual void Lock();

  // Virtual to test.
  virtual void Unlock();
  int id() const { return id_; }

  bool operator==(const Account& other) const {
    return id_ == other.id_ && balance_ == other.balance_;
  }

 private:
  int id_;
  int balance_;
  bool is_locked_;
};
