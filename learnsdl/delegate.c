#include "delegate.h"
#include <stdlib.h>

struct Delegate {
  DelegateCallback logic;
  DelegateCallback draw;
  DelegateCtx *ctx;
};

Delegate *delegate_new(DelegateCallback logic, DelegateCallback draw,
                       DelegateCtx *ctx) {
  return delegate_init(malloc(sizeof(Delegate)), logic, draw, ctx);
}

Delegate *delegate_init(Delegate *delegate, DelegateCallback logic,
                        DelegateCallback draw, DelegateCtx *ctx) {

  (*delegate) = (Delegate){
      .logic = logic,
      .draw = draw,
      .ctx = ctx,
  };
  return delegate;
}

void delegate_invoke(Delegate *delegate) {
	delegate->logic(delegate->ctx);
	delegate->draw(delegate->ctx);
}
