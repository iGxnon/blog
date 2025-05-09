---
title: 跨域请求的三种方式
date: 2021-12-22
description: 同源策略的弊端
taxonomies:
  categories:
    - 编程
  tags:
    - 跨域
    - CORS
---

# 没有缘起

---

网上和这相关的文章都快被写烂了，这篇文章就做备忘吧...

# 方式一: JSONP

---

一种非常巧妙的方式，简述大概如下:

前置知识: script 标签会把 src 属性里的 js 原封不动的 copy 下来，并且 copy 下来的 js 和本地的 js 共享同一个全局作用域

浏览器这边构建一个回调函数

```javascript
function handleResponse(response) {
  // response is a json type
  // do something with ${response}
}
```

然后在 html 里插入这个，这里 callback 的值是上面的回调函数

```html
<script src="http://www.example.com/jsonp/?callback=handleResponse"></script>
```

这样就好了，接下来服务端这边

```go
func jsonp(context *gin.Context) {
	var json string
	// do something to json ...

	callbackFunc := context.DefaultQuery("callback", "handleResponse")
	ret := fmt.Sprintf("%s(%s);", callbackFunc, json)
	context.String(http.StatusOK, ret)
}
```

返回 `${callback}(${json})`，相当于把 json 包装到${callback}函数里返回给浏览器，然后在浏览器里触发回调函数

优点就是简单灵巧，缺点就是只能发送 GET，无法检测是否请求失败

```go
// gin自带处理JSONP的方法

type Snitch struct {
	Id       int    `json:"id"`
	Name     string `json:"name"`
	Password string `json:"password"`
}

func jsonp(context *gin.Context) {
	snitch := model.Snitch{
        Id: 114514,
        Name: "哼哼",
        Password: "114514_24",
	}
	context.JSONP(http.StatusOK, snitch)
}

// 源码
// JSONP serializes the given struct as JSON into the response body.
// It adds padding to response body to request data from a server residing in a different domain than the client.
// It also sets the Content-Type as "application/javascript".
//func (c *Context) JSONP(code int, obj interface{}) {
//	callback := c.DefaultQuery("callback", "")
//	if callback == "" {
//        c.Render(code, render.JSON{Data: obj})
//        return
//	}
//	c.Render(code, render.JsonpJSON{Callback: callback, Data: obj})
//}
```

效果

![](jsonp.png)

# 方式二: WebSocket

---

首先要知道的是，**同源策略**并没有从根本上否决了跨域请求，只是阻止了 AJAX 请求，那么，为什么一定要使用 HTTP 协议来获取异源数据呢？

**HTTP 的能力是有极限哒，所以我不用 HTTP 了！JOJO！**

## 切换协议

简单流程:

浏览器发送一个换成 WebSocket 协议的 GET 请求

```txt
GET /chat HTTP/1.1
Host: server.example.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==
Sec-WebSocket-Protocol: chat, superchat
Sec-WebSocket-Version: 13
Origin: http://example.co
```

服务端回应 OK

```txt
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=
Sec-WebSocket-Protocol: chat
```

浏览器再发送一个收到 OK 的请求，三次握手成功

## 使用 WebSocket

浏览器这边

```javascript
// 实例化一个websocket对象
var ws = new WebSocket("wss://www.example.com");
ws.send("Hi!"); // 发送消息给服务器

// 如果想知道有没有发送成功 使用 Socket.bufferedAmount 字段，返回未发送的字节数量

// 接收消息
ws.onmessage = function (evt) {
  var received_msg = evt.data;
  alert("数据已接收...");
};
```

服务端这边

websocket 工具类

摘自 [https://www.jianshu.com/p/f058fdbdea58](https://www.jianshu.com/p/f058fdbdea58) (加了注释方便自己理解)

```go
// Package ws is to define a websocket server and client connect.
// Author: Arthur Zhang
// Create Date: 20190101
package ws

import (
    "encoding/json"

    "github.com/gorilla/websocket"
)

// ClientManager is a websocket manager
// 所有websocket连接的一个管理器
type ClientManager struct {
    Clients    map[*Client]bool // 保持连接的Client状态
    Broadcast  chan []byte // 接收到的数据channel, 可反序列化成 Message
    Register   chan *Client // 曾经连接过的Client
    Unregister chan *Client // 需要断开连接的 Client
}

// Client is a websocket client
type Client struct {
    ID     string
    Socket *websocket.Conn
    Send   chan []byte
}

// Message is an object for websocket message which is mapped to json type
type Message struct {
    Sender    string `json:"sender,omitempty"`
    Recipient string `json:"recipient,omitempty"`
    Content   string `json:"content,omitempty"`
}

// Manager define a ws server manager
var Manager = ClientManager{
    Broadcast:  make(chan []byte),
    Register:   make(chan *Client),
    Unregister: make(chan *Client),
    Clients:    make(map[*Client]bool),
}

// Start is to start a ws server
func (manager *ClientManager) Start() {
    for {
        select {
        case conn := <-manager.Register:
            // 初始化连接
            manager.Clients[conn] = true
            jsonMessage, _ := json.Marshal(&Message{Content: "/A new socket has connected."})
            manager.Send(jsonMessage, conn)
        case conn := <-manager.Unregister:
            // 断开连接
            if _, ok := manager.Clients[conn]; ok {
                close(conn.Send)
                delete(manager.Clients, conn)
                jsonMessage, _ := json.Marshal(&Message{Content: "/A socket has disconnected."})
                manager.Send(jsonMessage, conn)
            }
        case message := <-manager.Broadcast:
            for conn := range manager.Clients {
                select {
                case conn.Send <- message:
                default: // 如果这个Client的Send channel被关闭了，说明这个Client退出了连接
                    close(conn.Send)
                    delete(manager.Clients, conn)
                }
            }
        }
    }
}

// Send is to send ws message to ws client
// ignore 忽略掉的 Client
func (manager *ClientManager) Send(message []byte, ignore *Client) {
    for conn := range manager.Clients {
        if conn != ignore {
            conn.Send <- message
        }
    }
}

// Read 循环监听来自客户端的消息
func (c *Client) Read() {
    // 当连接断开时
    defer func() {
        Manager.Unregister <- c
        c.Socket.Close()
    }()

    for {
        _, message, err := c.Socket.ReadMessage()
        if err != nil {
            Manager.Unregister <- c
            c.Socket.Close()
            break
        }
        // 序列化成字符数组并写入到 Broadcast 内
        jsonMessage, _ := json.Marshal(&Message{Sender: c.ID, Content: string(message)})
        Manager.Broadcast <- jsonMessage
    }
}

func (c *Client) Write() {
    defer func() {
        Manager.Unregister <- c
        c.Socket.Close()
    }()

    for {
        select {
        case message, ok := <-c.Send: // 取出一条消息并发送
            if !ok {
                c.Socket.WriteMessage(websocket.CloseMessage, []byte{})
                return
            }

            c.Socket.WriteMessage(websocket.TextMessage, message)
        }
    }
}

```

将协议升级成 websocket，摘自 [https://www.jianshu.com/p/f058fdbdea58](https://www.jianshu.com/p/f058fdbdea58)

```go
// WsPage is a websocket handler
func WsPage(c *gin.Context) {
    // change the reqest to websocket model
    // 大致意思就是允许所有的 Origin(不建议)， 更新协议并创建了一个 websocket 对象
    conn, error := (&websocket.Upgrader{CheckOrigin: func(r *http.Request) bool { return true }}).Upgrade(c.Writer, c.Request, nil)
    if error != nil {
        http.NotFound(c.Writer, c.Request)
        return
    }
    // websocket connect 初始化 Client
    // uuid 为第三方库，也可以自己整一个uuid生成函数
    client := &ws.Client{Id: uuid.NewV4().String(), Socket: conn, Send: make(chan []byte)}

    // 注册这个 Client
    ws.Manager.Register <- client

    // 开启 Read() 和 Write() 监听 Goroutine
    go client.Read()
    go client.Write()
}

// 然后就是注册路由 engine.GET("/ws", WsPage)
```

然后就可以调用 ClientManager 里的 Send() 方法发送消息了，ClientManager 里的 Broadcast 储存了来自 Clients 发过来的消息

# ⭐ 方式三: CORS

---

加了颗 ⭐ ，说明这是主要的解决方案

前面两个都算是"歪门邪道"

## 简单请求

请求方法只能是 `GET POST HEAD`其中的一个，HTTP Header 只能有以下内容

- Accept
- Accept-Language
- Content-Language
- Last-Event-ID
- Content-Type（只能为 application/x-www-form-urlencoded，multipart/form-data 和 text/plain 其中一种）

然后支持 CORS 的浏览器就会在 HTTP 头信息里加入一个 Origin 字段指定一个`源`(就一 URL)，类似这样

```txt
GET /cors HTTP/1.1
Origin: http://api.example.com
Host: xxx
Accept-Language: en-US
Connection: keep-alive
User-Agent: Mozilla/5.0 ...
```

这些操作都是支持 CORS 的浏览器自动帮用户整的，不需要前端工程师自己加

服务端这边返回的 HTTP 结果必须得满足一下字段要求

- Access-Control-Allow-Origin（必须）：表示允许的 Origin 字段，可以是\*，通常就是 CORS 请求中的 Origin 字段
- Access-Control-Allow-Credentials（可选）：填 true，表示允许浏览器发送 Cookie，不想发送就删掉这个字段
- Access-Control-Expose-Headers（可选）：用 `,` 隔开，表示浏览器允许访问的 Header

### 代码部分

浏览器这边

```javascript
var xhr = new XMLHttpRequest();
xhr.withCredentials = true; // 想要发送 Cookie 得先开启这个
// 就算开启了也不一定能发送，如果返回HTTP字段里没有 Access-Control-Allow-Credentials 也白搭
// 并且服务器返回的 Access-Control-Allow-Origin 不能是 * ,不然也不行
```

服务端这边

```go
// 中间件
// 只支持简单请求
func cors() gin.HandlerFunc {
	allowOrigins := []string{"http://api.foo.com", "http://api.bar.com"}
	return func(ctx *gin.Context) {
        origin := ctx.GetHeader("Origin")
        for _, allowed := range allowOrigins {
        	if origin == allowed {
                ctx.Header("Access-Control-Allow-Origin", origin)
                ctx.Header("Access-Control-Allow-Credentials", "true")
                ctx.Header("Access-Control-Expose-Headers", "MyHeader, MyHeaderTwo") // 允许访问 MyHeaderOne 和 MyHeaderTwo
                ctx.Header("MyHeaderOne", "1") // 自定义 Header
                ctx.Header("MyHeaderTwo", "2")

                ctx.Next()
                return
        	}
        }
        // 正常返回, 但浏览器未查询到有 Access-Control-Allow-Origin 就会报错
        ctx.AbortWithStatus(200)
        // 处理请求
        ctx.Next()
	}
}
```

## 非简单请求

不是简单请求就是非简单请求

浏览器首先会发送一个 OPTIONS 请求，询问是否有获取资源的权限(`预检`)

```txt
OPTIONS /cors HTTP/1.1
Origin: http://api.foo.com
Access-Control-Request-Method: PUT
Access-Control-Request-Headers: X-Custom-Header
Host: api.bar.com
Accept-Language: en-US
Connection: keep-alive
User-Agent: Mozilla/5.0...
```

- Origin：和简单请求一样
- Access-Control-Request-Method：请求是否允许这个 HTTP 方法
- Access-Control-Request-Headers：请求是否允许浏览器会额外发送的 Header，有多个用 `,` 隔开

然后服务端收到请求后检查这些字段是否符合要求，返回以下消息

```txt
HTTP/1.1 200 OK
Date: Mon, 01 Dec 2008 01:15:39 GMT
Server: Apache/2.0.61 (Unix)
Access-Control-Allow-Origin: http://api.foo.com
Access-Control-Allow-Methods: GET, POST, PUT
Access-Control-Allow-Headers: X-Custom-Header
Access-Control-Allow-Credentials: true
Access-Control-Max-Age: 1728000
Content-Type: text/html; charset=utf-8
Content-Encoding: gzip
Content-Length: 0
Keep-Alive: timeout=2, max=100
Connection: Keep-Alive
Content-Type: text/plain
```

- Access-Control-Allow-Origin：和简单请求一样，可以填 \*
- Access-Control-Allow-Methods：允许浏览器请求用的的 HTTP 方法，用 `,` 隔开多个
- Access-Control-Allow-Headers：支持浏览器额外发送的 Header，用 `,` 隔开多个
- Access-Control-Allow-Credentials(可选)：和简单请求一样，填 true
- Access-Control-Max-Age(可选)：这个预检的时效，在该时效内就不用再发送预检请求了

注意：**这里的** `Access-Control-Allow-Headers` **和简单请求中的** `Access-Control-Expose-Headers` **不一样，这里的表示服务器可以读取的 Header，允许浏览器可以发送哪些 Header，简单请求表示的是允许浏览器可以读取哪些 Header**

浏览器看到了返回信息有以上这些字段，就认为服务器同意了预检，反之，就会报错

通过了预检后，浏览器就会发送正常的非简单请求

```txt
PUT /cors HTTP/1.1
Origin: http://api.foo.com
Host: api.bar.com
X-Custom-Header: value
Accept-Language: en-US
Connection: keep-alive
User-Agent: Mozilla/5.0...
```

- Origin：和简单请求一样
- X-Custom-Header：预检是所请求的 Header

然后服务端每次回复都需要带上

- Access-Control-Allow-Origin：和简单请求的一样，可以是\*

### 代码部分

浏览器这边

```javascript
var url = "http://api.bar.com/cors";
var xhr = new XMLHttpRequest();
xhr.open("PUT", url, true);
xhr.setRequestHeader("X-Custom-Header", "value");
xhr.send();
```

和简单请求一样，浏览器会自动完成`预检`并发送正常请求，如果预检不成功就会报错（调用 xhr.onerror 即可获取错误）

服务端这边

```go
// 中间件
// 支持简单请求和非简单请求
func cors() gin.HandlerFunc {
	return func(ctx *gin.Context) {
        method := ctx.Request.Method

        // 同意所有源
        ctx.Header("Access-Control-Allow-Origin", "*")

        // 处理并放行所有 OPTIONS 请求
        if method == "OPTIONS" {
            ctx.Header("Access-Control-Allow-Methods", "PUT, GET, DELETE, POST")
            ctx.Header("Access-Control-Allow-Headers", "X-Custom-Header")
            ctx.Header("Access-Control-Allow-Credentials", "true")
            ctx.Header("Access-Control-Max-Age", "1728000")
            ctx.AbortWithStatus(http.StatusNoContent)
        }

        ctx.Next()
	}
}
```

## 第三方库 cors

一个 gin 拓展插件，[github.com/gin-contrib/cors](github.com/gin-contrib/cors)

具体怎么用还是看看源码吧，(其实也自己实现 CORS 跨域也不麻烦)
