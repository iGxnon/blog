---
title: 图解OAuth2.0
date: 2021-12-22
extra:
  add_toc: true
taxonomies:
  categories:
    - 笔记
  tags:
    - OAuth
    - web
---

# 前言

---

> OAuth2.0 的标准是 [RFC6749](https://tools.ietf.org/html/rfc6749)
> 当然全是英文...

> 常见的使用微信登录，使用 Github 登录等操作使用的就是 OAuth 标准

# 图解

---

## 授权码式(最常见)

> 适用于前后端分离的 web 应用
> appsecret 和 token 均保存在后端，因为放在浏览器内保存会有危险(这就是为什么不直接返回 token 而是返回 code)

![](https://image-1259160349.cos.ap-chengdu.myqcloud.com/img/%E6%8E%88%E6%9D%83%E7%A0%81%E5%BC%8F.png#crop=0&crop=0&crop=1&crop=1&id=UqJDG&originHeight=1124&originWidth=1320&originalType=binary∶=1&rotation=0&showTitle=false&status=done&style=none&title=)

## 隐藏式

> 适用于没有后端的 web 应用
> 这种授权方式很不安全，通常适用于对安全性要求低的场景，授权时间很短

![](https://image-1259160349.cos.ap-chengdu.myqcloud.com/img/%E9%9A%90%E8%97%8F%E5%BC%8F.png#crop=0&crop=0&crop=1&crop=1&id=u5KE7&originHeight=1124&originWidth=1252&originalType=binary∶=1&rotation=0&showTitle=false&status=done&style=none&title=)

## 密码式

> 很不安全，只有十分相信这个 app 的情况下再去登录

![](https://image-1259160349.cos.ap-chengdu.myqcloud.com/img/%E5%AF%86%E7%A0%81%E5%BC%8F_.png#crop=0&crop=0&crop=1&crop=1&id=fGkKL&originHeight=826&originWidth=1574&originalType=binary∶=1&rotation=0&showTitle=false&status=done&style=none&title=)

## 凭证式

> 这不是针对用户授权登录的，而是针对服务端的，获得的 token 能请求到的是第三方服务资源，而不是用户的资源，所以可能存在多个用户共享一个 token

![](https://image-1259160349.cos.ap-chengdu.myqcloud.com/img/%E5%87%AD%E8%AF%81%E5%BC%8F.png#crop=0&crop=0&crop=1&crop=1&id=xHjce&originHeight=726&originWidth=1446&originalType=binary∶=1&rotation=0&showTitle=false&status=done&style=none&title=)

# 使用 Github OAuth Server 的一个 Demo

---

> 这里是前后端不分离的
> 源代码: [https://gitee.com/iGxnon/oauth-github-test](https://gitee.com/iGxnon/oauth-github-test)

> 效果

![](https://image-1259160349.cos.ap-chengdu.myqcloud.com/img/ScreenFlow.gif#crop=0&crop=0&crop=1&crop=1&id=NnilX&originHeight=1080&originWidth=1920&originalType=binary∶=1&rotation=0&showTitle=false&status=done&style=none&title=)
