---
title: 浅谈加密
date: 2021-11-15
description: 介绍了几个常见的加密方法
taxonomies:
  categories:
    - 编程
    - 数学
  tags:
    - 加密
    - RSA
    - AES
---

# 缘起

---

一次作业，写一个后端登录系统，涉及到了对用户密码进行加密传输

脑子里突然蹦出来这个**RSA 非对称加密算法**（也是我接触的第一个加密算法

# 浅谈

---

目前加密算法大概可以分为三类

| **种类**   | **加密规则**                                                                                                             | **破解方式**                                       |
| ---------- | ------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------- |
| 对称加密   | 根据一个固定的规则(密钥)进行加密和解密                                                                                   | 根据密文可以破解密钥，密钥传输时截获密钥           |
| 非对称加密 | 两个密钥:公钥和私钥，私钥只保留给自己，公钥交给他人;公钥私钥可以互相加密解密，通常公钥破解难度比私钥小，加密速度比私钥快 | 难以破解                                           |
| Hash       | 明文到密文不可逆(很难破解)                                                                                               | 不带盐 hash 可以用彩虹表和碰撞破解，带盐的难以破解 |

## 对称加密

对称加密应用很广泛，大多数古典加密也属于对称加密（古典加密通常使用**移位**和**置换**，例如**凯撒密码**

缺点也十分明显，**双方**必须保证密钥严格安全，毕竟加密方式都泄露了那密文就和明文差不多了

甲方制定的密钥必须给乙方才能通信，密钥的传输和保存很难保证安全

## 非对称加密

这也是本文的重点 **RSA 非对称加密**

> 1976 年，两位美国计算机学家 Whitfield Diffie 和 Martin Hellman，提出了一种崭新构思，可以在不直接传递密钥的情况下，完成解密。这被称为`Diffie-Hellman密钥交换算法`。这个算法启发了其他科学家。人们认识到，加密和解密可以使用不同的规则，只要这两种规则之间存在某种对应关系即可，这样就避免了直接传递密钥。

**这种加密方法被称为**`非对称加密`

> 1977 年，三位数学家 Rivest、Shamir 和 Adleman 设计了一种算法，可以实现非对称加密。这种算法用他们三个人的名字命名，叫做`RSA算法`。从那时直到现在，RSA 算法一直是最广为使用的"非对称加密算法"。毫不夸张地说，只要有计算机网络的地方，就有 RSA 算法。

你可以发现目前 OpenSSH 使用的就是 RSA 密钥认证，Github，Gitee 等代码托管平台都可以添加 SSH 公钥认证权限

还有语雀的 SSL 证书

![](yuque.png)

## Hash

Hash 就是散列函数，加密后的密文长度固定，根据排列组合知识，也就是说密文对应的原文不唯一，也就是说无法被解密

这种加密典型的**原文加密到密文容易，密文解密到原文不可能实现**(如果能实现就说明宇宙所有事物都能用一串 hash 码表达，也就是无穷可以被定量，这是不可能的)

通常应用场景就是在数据库里储存隐私的 Hash 码

常见的有 SHA 系列，MD5(1996 年后被专家证明出有缺点，对数据安全性较高的建议使用 SHA-2)，

- **碰撞(collision)**

根据上文，既然 hash 的输入有无限种可能，而输出只有有限的可能，那么绝对会存在两个明文对应的密文一样，如果两个明文对应的密文一样，这就发生了**碰撞**

所以，根据这个，破密者可以预先存储一个**密文->明文**的表，当发现其中某个密文和要破解密文一样，那么即使这个密文对应的明文和破解的密文对应的明文不是一样的，这个明文也能完成真正的明文大多数作用(比如作为密码进行登陆)

- **彩虹表(rainbow table)**

储存一个完完整整的**密文->明文**的散列表是不合理的，这样会占用很多数据库的空间，而且检索起来会变得非常慢

彩虹表采用的就是以时间复杂度来换取空间复杂度的思路

**彩虹表前身: 预计算的哈希链** > **约简函数(Reduction Function)**

约简函数的定义域和值域刚好和要破译的 Hash 函数相反。也就是说它可以将密文转换成和明文相同格式的内容，但得出来的这个明文不一定计算 Hash 后就和密文一样

![](reduction_fn.png)

过程

- 随机生成一个字符串 aaaaaa
- 对其求 Hash 得 281DAF40
- 对上一步结果求约简得 sgfnyd
- 重复上面的步骤得到一个 kiebgt


**储存的时候只需要储存 aaaaaa 和 kiebgt 就可以了，剩下的都可以计算 Hash 和约简来得到** **这样就可以省下很大的空间了**

和 Hash 一样，约减函数也会发生碰撞，当发生碰撞的时候，两条链就重叠了

如:

A 链: aaa --> **3A8GH --> ehg** --> 6J94BD
 
B 链: bbb --> **4CG8D --> ehg** --> 6J94BD
 
在对 3A8GH 和 4CG8D 计算约简结果时发生了碰撞，得到了相同的 ehg，那么这两条链能储存的信息就变少了，也就浪费了空间

**彩虹表**

![](rainbow_table.png)

看起来像一个预计算哈希链组成的一个集合

相比于纯预计算哈希链集合

- 每条链的 R 函数都不一样，降低发生碰撞的概率
- 如果发生了碰撞，那么末节点肯定是一样的，舍去其中一条(或者检索出碰撞点用另外一个 R 函数继续计算


**如何使用它们**

- 拿到一串密文，先对它求一次约简，看看有没有命中其中某条链的末节点
- 如果没有命中并且计算的次数小于链长度就对上一步操作求一次 Hash，带回上一步
- 如果命中了反过来对末节点回溯得到解密结果
- 计算次数大于链长度就停止，说明破解不了

**带盐(salt)**

咱吃饭的时候总不能不放盐吧，虽然这里的盐(salt)和真实的盐(NaCl)没有关系，但在某些方面有点类似，(比如都是附加成分?)

计算 Hash 的时候不仅仅就只针对这串明文，给明文带一点其他的元素，比如密码明文加上用户年龄求一次 Hash，这个盐就是年龄 (当然前提你得保密)

**盐值就是不确定因素，对每个 Hash 计算可能都不一样**

带上了盐值，Hash 规则就变了，破译者还不知道它是按照什么规则变的，自然就无法用彩虹表等方法破译了

![](nice.gif)

# RSA 加密过程

---

## 浅要分析

RSA 加密原理主要基于目前数学领域对质数的研究匮乏，分解一个很大的数的质因数远远比从质因数得出这个数复杂得多

下面的过程中，P 和 Q 很大时，从 N 分解出 P 和 Q 将非常困难，分解不出来这样就得不到私钥，得不到私钥就破解不了(默认没有其他不用私钥就能破解的方法，至少目前没有)

RSA 从问世这么多年以来，经过无数次验证，攻击考验，始终表现出优异的安全性，所以在非对称加密方向上被广泛使用

## 简单加密过程

![](encr.png)

**例子**

找出 P=3 Q=11
计算得出 N = 33，Φ(N) = 2\*10 = 20
公钥 E∈(1,20)，不妨取其为 3
计算可得私钥 D=7

加密过程 (以 2 为例)
C = 2^3 % 33 = 8
解密过程
M = 8^7 % 33 = 2

## 代码实现(Golang 为例)

```go
import (
	"crypto/rsa"
	"crypto/rand"
	"crypto/x509"
	"os"
	"encoding/pem"
	"fmt"
)

//生成RSA私钥和公钥，保存到文件中

func GenerateRSAKey(bits int){

	//GenerateKey函数使用随机数据生成器random生成一对具有指定字位数的RSA密钥
	//Reader是一个全局、共享的密码用强随机数生成器
	privateKey, err := rsa.GenerateKey(rand.Reader, bits)

	if err!=nil{
		panic(err)
	}

	//保存私钥
	//通过x509标准将得到的ras私钥序列化为ASN.1 的 DER编码字符串
	X509PrivateKey := x509.MarshalPKCS1PrivateKey(privateKey)

	//使用pem格式对x509输出的内容进行编码
	//创建文件保存私钥
	privateFile, err := os.Create("private.pem")

	if err!=nil{
		panic(err)
	}

	defer privateFile.Close()

	//构建一个pem.Block结构体对象
	privateBlock:= pem.Block{Type: "RSA Private Key",Bytes:X509PrivateKey}

	//将数据保存到文件

	pem.Encode(privateFile,&privateBlock)

	//保存公钥
	//获取公钥的数据
	publicKey:=privateKey.PublicKey

	//X509对公钥编码
	X509PublicKey,err:=x509.MarshalPKIXPublicKey(&publicKey)

	if err!=nil{
		panic(err)
	}

	//pem格式编码
	//创建用于保存公钥的文件
	publicFile, err := os.Create("public.pem")

	if err!=nil{
		panic(err)
	}

	defer publicFile.Close()

	//创建一个pem.Block结构体对象
	publicBlock:= pem.Block{Type: "RSA Public Key",Bytes:X509PublicKey}

	//保存到文件
	pem.Encode(publicFile,&publicBlock)

}

//RSA加密

func RSA_Encrypt(plainText []byte,path string)[]byte{

	//打开文件
	file,err:=os.Open(path)
	if err!=nil{
		panic(err)
	}

	defer file.Close()

	//读取文件的内容
	info, _ := file.Stat()
	buf:=make([]byte,info.Size())
	file.Read(buf)

	//pem解码
	block, _ := pem.Decode(buf)

	//x509解码
	publicKeyInterface, err := x509.ParsePKIXPublicKey(block.Bytes)

	if err!=nil{
		panic(err)
	}

	//类型断言
	publicKey:=publicKeyInterface.(*rsa.PublicKey)

	//对明文进行加密
	cipherText, err := rsa.EncryptPKCS1v15(rand.Reader, publicKey, plainText)

	if err!=nil{
		panic(err)
	}

	//返回密文
	return cipherText

}

//RSA解密

func RSA_Decrypt(cipherText []byte,path string) []byte{

	//打开文件
	file,err:=os.Open(path)
	if err!=nil{
		panic(err)
	}
	defer file.Close()

	//获取文件内容
	info, _ := file.Stat()
	buf:=make([]byte,info.Size())
	file.Read(buf)

	//pem解码
	block, _ := pem.Decode(buf)

	//X509解码
	privateKey, err := x509.ParsePKCS1PrivateKey(block.Bytes)

	if err!=nil{
		panic(err)
	}

	//对密文进行解密
	plainText,_:=rsa.DecryptPKCS1v15(rand.Reader,privateKey,cipherText)

	//返回明文
	return plainText

}
```

测试

```go
func main(){

 //生成密钥对，保存到文件
 GenerateRSAKey(2048)
 message:=[]byte("hello world")

 //加密
 cipherText:=RSA_Encrypt(message,"public.pem")
 fmt.Println("加密后为：",string(cipherText))

 //解密
 plainText := RSA_Decrypt(cipherText, "private.pem")
 fmt.Println("解密后为：",string(plainText))

}
```

测试结果

![](result.png)

密文，签名，密钥的字符串加密方式

以密文为例，加密后的结果看起来是一团乱码，有很多无法表示的符号，储存时一般对其字符串加密
一般常用 Hex 和 Base64

Hex 字符串加密

```go
hex.DecodeString(s string) // 解密
hex.EncodeToString(src []byte) string // 加密
```

Base64 字符串加密

```go
base64.StdEncoding.DecodeString(s string) ([]byte, error) // 解密
base64.StdEncoding.EncodeToString(src []byte) string // 加密
```

# AES&DES

---

非对称加密固然安全，但加密解密开销是很大的，通常上就使用非对称加密来传输对称加密的密钥 key，然后再使用对称加密来加密交流的信息

AES 和 DES 是对称加密中常用的算法
至于具体原理就不能深入了（我不会

贴个链接 AES 原作者论文: [https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf](https://csrc.nist.gov/csrc/media/projects/cryptographic-standards-and-guidelines/documents/aes-development/rijndael-ammended.pdf)

## 代码实现

可以看到在 golang 中实现 aes 和 des 基本上没区别

```go

// PKCS7Padding 把 ciphertext 补成 blockSize 整数倍
func PKCS7Padding(ciphertext []byte, blockSize int) []byte {
	padding := blockSize - len(ciphertext)%blockSize
	padtext := bytes.Repeat([]byte{byte(padding)}, padding) // 缺多少个字节就补多少个 例：缺5个补5个5
	return append(ciphertext, padtext...)
}

func PKCS7UnPadding(origData []byte) []byte {
	length := len(origData)
	unpadding := int(origData[length-1])   // 拿到最后一个字节(数字)，从而得知之前补了多少个字节
	return origData[:(length - unpadding)] // 获取原本未补齐的 origData
}

// AesEncrypt AES加密
func AesEncrypt(origData, key []byte) ([]byte, error) {
	block, err := aes.NewCipher(key) // 通过 key 确定 BlockSize
	if err != nil {
		return nil, err
	}
	blockSize := block.BlockSize()
	origData = PKCS7Padding(origData, blockSize)                // 把 origData 补齐
	blockMode := cipher.NewCBCEncrypter(block, key[:blockSize]) // 通过 BlockSize 确定 BlockMode
	encrypted := make([]byte, len(origData))
	blockMode.CryptBlocks(encrypted, origData)
	return encrypted, nil
}

// AesDecrypt AES解密
func AesDecrypt(encrypted, key []byte) ([]byte, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}
	blockSize := block.BlockSize()
	blockMode := cipher.NewCBCDecrypter(block, key[:blockSize])
	origData := make([]byte, len(encrypted))
	blockMode.CryptBlocks(origData, encrypted)
	origData = PKCS7UnPadding(origData)
	return origData, nil
}

// DesEncrypt DES加密
func DesEncrypt(origData, key []byte) ([]byte, error) {
	block, err := des.NewCipher(key)
	if err != nil {
		return nil, err
	}
	blockSize := block.BlockSize()
	origData = PKCS7Padding(origData, blockSize)
	blockMode := cipher.NewCBCEncrypter(block, key[:blockSize])
	encrypted := make([]byte, len(origData))
	blockMode.CryptBlocks(encrypted, origData)
	return encrypted, nil
}

// DesDecrypt DES解密
func DesDecrypt(encrypted, key []byte) ([]byte, error) {
	block, err := des.NewCipher(key)
	if err != nil {
		return nil, err
	}
	blockSize := block.BlockSize()
	blockMode := cipher.NewCBCDecrypter(block, key[:blockSize])
	origData := make([]byte, len(encrypted))
	blockMode.CryptBlocks(origData, encrypted)
	origData = PKCS7UnPadding(origData)
	return origData, nil
}

```

## 速度对比

```go

func TestEncrypt(t *testing.T) {
	aesKey := []byte("1234567812345678") // 密钥是 16 的整数倍
	desKey := []byte("12345678")
	text := []byte("Hello World!")

	start := time.Now()

	aesEncrypt, err := AesEncrypt(text, aesKey)
	if err != nil {
		return
	}
	end1 := time.Now().Sub(start)

	desEncrypt, err := DesEncrypt(text, desKey)
	if err != nil {
		return
	}
	end2 := time.Now().Sub(start)

	fmt.Println(string(aesEncrypt), "用时", end1.Nanoseconds())
	fmt.Println(string(desEncrypt), "用时", end2.Nanoseconds())

	fmt.Println("加密对比", "AES 加密速度是 DES 的", float64(end2.Nanoseconds()) / float64(end1.Nanoseconds()), "倍")

	aesDecrypt, err := AesDecrypt(aesEncrypt, aesKey)
	if err != nil {
		return
	}
	end3 := time.Now().Sub(start)

	desDecrypt, err := DesDecrypt(desEncrypt, desKey)
	if err != nil {
		return
	}
	end4 := time.Now().Sub(start)

	fmt.Println(string(aesDecrypt), "用时", end3.Nanoseconds())
	fmt.Println(string(desDecrypt), "用时", end4.Nanoseconds())

	fmt.Println("解密对比", "AES 解密速度是 DES 的", float64(end4.Nanoseconds()) / float64(end3.Nanoseconds()), "倍")

}
```

![](test.png)

咳咳咳，不要在意壁纸

多次加密测试后，AES 的加密速度一般是 DES 的 1.5 ～ 2.5 倍，解密速度倒是没啥区别

# 参考&引用

---

- 密码破解的利器——彩虹表（rainbow table）[https://www.jianshu.com/p/732d9d960411](https://www.jianshu.com/p/732d9d960411)
- RSA 原理(一) [http://www.ruanyifeng.com/blog/2013/06/rsa_algorithm_part_one.html](http://www.ruanyifeng.com/blog/2013/06/rsa_algorithm_part_one.html)
- RSA 原理(二) [https://www.ruanyifeng.com/blog/2013/07/rsa_algorithm_part_two.html](https://www.ruanyifeng.com/blog/2013/07/rsa_algorithm_part_two.html)
- 小白也能看懂的 RSA 加密原理 [https://blog.csdn.net/jijianshuai/article/details/80582187](https://blog.csdn.net/jijianshuai/article/details/80582187)
- Golang RSA 加密签名实践 [https://m.jb51.net/article/211501.htm](https://m.jb51.net/article/211501.htm)
