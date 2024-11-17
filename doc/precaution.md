# 注意事项

在使用 Qt WebAssembly 时，由于浏览器上的 web 应用不同于客户端，容易出现各种问题，下面我列举出相应的问题，方便之后查找解决方案。

**跨域访问**

若服务端没有允许跨域访问，浏览器会进行拦截，我这里使用的是 drogon 进行后端开发，需要在返回响应报文加上一个字段。

```cpp
Json::Value ret;
auto resp=HttpResponse::newHttpJsonResponse(ret);
resp->setStatusCode(k200OK);
resp->addHeader("Access-Control-Allow-Origin","*");
callback(resp);
```

**session**

在使用同一个 QNetworkAccessManager 进行请求的时候，客户端每次请求都在一个 session 中，而编译成 WebAssembly 之后每次请求都是一个单独的 session，这一点很奇怪，还没有找到解决方案。