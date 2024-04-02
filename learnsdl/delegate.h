#ifndef DELEGATE_H
#define DELEGATE_H 1

typedef struct Delegate Delegate;
typedef void DelegateCtx;
typedef void (*DelegateCallback)(DelegateCtx *);

Delegate *delegate_new(DelegateCallback logic, DelegateCallback draw,
                       DelegateCtx *ctx);
Delegate *delegate_init(Delegate *delegate, DelegateCallback logic,
                        DelegateCallback draw, DelegateCtx *ctx);
void delegate_invoke(Delegate *);

#endif
