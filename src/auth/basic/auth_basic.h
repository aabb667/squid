/*
 * auth_basic.h
 * Internal declarations for the basic auth module
 */

#ifndef __AUTH_BASIC_H__
#define __AUTH_BASIC_H__

#include "auth/Gadgets.h"
#include "auth/User.h"
#include "auth/UserRequest.h"
#include "auth/Config.h"
#include "helper.h"

#define DefaultAuthenticateChildrenMax  32	/* 32 processes */

/** queue of auth requests waiting for verification to occur */
class BasicAuthQueueNode
{

public:
    BasicAuthQueueNode *next;
    AuthUserRequest::Pointer auth_user_request;
    RH *handler;
    void *data;
};

class BasicUser : public AuthUser
{

public:
    MEMPROXY_CLASS(BasicUser);

    BasicUser(Auth::Config *);
    ~BasicUser();
    bool authenticated() const;
    void queueRequest(AuthUserRequest::Pointer auth_user_request, RH * handler, void *data);
    void submitRequest(AuthUserRequest::Pointer auth_user_request, RH * handler, void *data);

    bool valid() const;

    /** Update the cached password for a username. */
    void updateCached(BasicUser *from);
    virtual int32_t ttl() const;

    char *passwd;

    BasicAuthQueueNode *auth_queue;

private:
    AuthUserRequest::Pointer currentRequest;
};

MEMPROXY_CLASS_INLINE(BasicUser);

namespace Auth {
namespace Basic {

/** Basic authentication configuration data */
class Config : public Auth::Config
{
public:
    Config();
    ~Config();
    virtual bool active() const;
    virtual bool configured() const;
    virtual AuthUserRequest::Pointer decode(char const *proxy_auth);
    virtual void done();
    virtual void rotateHelpers();
    virtual void dump(StoreEntry *, const char *, Auth::Config *);
    virtual void fixHeader(AuthUserRequest::Pointer, HttpReply *, http_hdr_type, HttpRequest *);
    virtual void init(Auth::Config *);
    virtual void parse(Auth::Config *, int, char *);
    void decode(char const *httpAuthHeader, AuthUserRequest::Pointer);
    virtual void registerWithCacheManager(void);
    virtual const char * type() const;

public:
    char *basicAuthRealm;
    time_t credentialsTTL;
    int casesensitive;
    int utf8;

private:
    char * decodeCleartext(const char *httpAuthHeader);
};

} // namespace Basic
} // namespace Auth

#endif /* __AUTH_BASIC_H__ */
