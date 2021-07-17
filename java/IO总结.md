# IO总结
## Java中常用的IO处理类
### 分类
Java中常用的IO类可以按照不同的方式进行划分，按照处理的对象划分，就可以划分为字节流和字符流`InputStream/OutputStream`和`Reader/Writer`；按照功能实现划分，可以分为节点流和过滤流。
**节点流**
节点流是根据输入输出源不同实现的不同类，File文件、char数据、字符串、pipe通道等分为不同的实现类。
+ FileReader/FileInputStream/FileWriter/FileOutputStream 用于文件
+ CharArrayReader/ByteArrayInputStream/CharArrayWriter/ByteArrayOutputStream用于从程序中的字符数组输入
+ StringReader/StringBufferInputStream/StringWriter 用于从程序中的字符串输入
+ PipedReader/PipeInputStream用于读取从另一个线程中的数据
+ PipedWriter/PipeOutputStream/PipeWriter/PipedOutputStream 写入管道的数据，不同线程间交互

**过滤流**
使用装饰者模式，基于节点流，提供很多的额外能力类。
+ 缓冲流 BufferedInputStream/BufferedOutputStream/BufferedReader/BufferedWriter，字符缓冲流分别提供了读取和写入一行的方法ReadLine和NewLine方法
+ 字符字节转换 InputStreamReader / OutputStreamWriter。其中，InputStreamReader需要与InputStream“套接”，OutputStreamWriter需要与OutputStream“套接”
+ 数据流 DataInputStream和DataOutputStream分别继承自InputStream和OutputStream，需要“套接”在InputStream和OutputStream类型的节点流之上
+ 对象流 ObjectInputStream和ObjectOutputStream，本身这两个方法没什么，但是其要写出的对象有要求，该对象必须实现Serializable接口，来声明其是可以序列化的

*TODO 后续有时间可以补充下这些类的细节和JVM底层的实现原理*
### `InputStream`&`OutputStream`
这是IO操作的基础类，提供了InputStream和OutputStream，按照字节读取数据，他们都是抽象类；  从下面的代码中可以出现看出，基础的方法实现都是按照一个字节来读写的，<br>
IO的读写效率很低；
```java
package java.io;
public abstract class InputStream implements Closeable {
    // 返回下一个字符值：0-255，-1表示已经到文件结尾
    public abstract int read() throws IOException;
    
    public int read(byte b[], int off, int len) throws IOException {
        // read的基础方法
        // 方法将stream中的数据复制给数组b，off是本次读取b数组的起始位置，len是本次读取的长度，
        // 方法返回实际读取的字符数
        // 核心代码
        for (; i < len ; i++) {
                c = read();
                if (c == -1) {
                    break;
                }
                b[off + i] = (byte)c;
        }
    }
    
    // read的重载方法，简化输入参数
    public int read(byte b[]) throws IOException {
        return read(b, 0, b.length);
    }
    
    public long skip(long n) throws IOException {
        // 跳过n个数目的字符方法
        // 通过内部定义的一个byte数组，调用read方法读取stream数据，数据最大定义为：private static final int MAX_SKIP_BUFFER_SIZE = 2048
        // 方法返回实际跳过的字符数目
        // 核心代码
        while (remaining > 0) {
            nr = read(skipBuffer, 0, (int)Math.min(size, remaining));
            if (nr < 0) {
                break;
            }
            remaining -= nr;
        }
    }
    
    // stream中的字符总数，需要被子类重写
    public int available() throws IOException {
        return 0;
    }
    
    // 资源释释放方法
    public void close() throws IOException {}
    
    /** 这三个方法配套使用，下面的描述的InputStream的描述，实际要看各个子类的实现
     * mark方法不执行任何操作，只是标记输入流的位置，reset()方法重新定位到这个位置。
     * 如果方法markSupported返回true，则流将以某种方式记住在调用mark之后读取的所有字节，并准备在每次调用方法reset时再次提供相同的字节。
     * 但是，如果在调用复位之前从流中读取了超过readlimit个字节的数据，则该流根本不需要记住任何数据。
     */
    public synchronized void mark(int readlimit) {}
    public synchronized void reset() throws IOException {
        throw new IOException("mark/reset not supported");
    }
    public boolean markSupported() {
        return false;
    }
}
```
```java
package java.io;
public abstract class OutputStream implements Closeable, Flushable {
    // 按照字节写数据，int的低8位被写入，高位被丢弃
    public abstract void write(int b) throws IOException;
    
    public void write(byte b[], int off, int len) throws IOException {
        // 写数据的基本方法，将字节数组b的内容，从off开始的len个字符按照顺序调用write方法写入
        // 核心代码
        for (int i = 0 ; i < len ; i++) {
            write(b[off + i]);
        }
    }
    
    // write的重载方法，简化数组参数
    public void write(byte b[]) throws IOException {
        write(b, 0, b.length);
    }
    
    // stream中的数据立即被写入到目标中
    public void flush() throws IOException {
    }
    
    // 关闭stream，释放资源，关闭后就不能重新打开
    public void close() throws IOException {
    }
}
```
### `Reader`&`Writer`
InputStream和OutputStream按照字节读取，一些实际的字符处理可能会导致问题，为了方便人类理解的方式读取文本，提供了Reader和Writer，按照字符读取
```java
public abstract class Reader implements Readable, Closeable {
    // synchronize锁的对象
    protected Object lock;
    
    // 两个构造函数，提供的锁对象不同
    protected Reader() {
        this.lock = this;
    }
    
    protected Reader(Object lock) {
        if (lock == null) {
            throw new NullPointerException();
        }
        this.lock = lock;
    }
   
   // 需要实现，将输入数据已char格式写到cbuf中，是read的基础的方法，其他read方法都基于该方法，包括read()
   abstract public int read(char cbuf[], int off, int len) throws IOException;

    //ture保证不会阻塞输入，反之不保证
    public boolean ready() throws IOException {
        return false;
    }
    
   // 下面的方法作用和InputStream类似
    public void mark(int readAheadLimit) throws IOException {
        throw new IOException("mark() not supported");
    }
    public boolean markSupported() {
        return false;
    }
    public long skip(long n) throws IOException {
        //
    }
    public void reset() throws IOException {
        throw new IOException("reset() not supported");
    }
    abstract public void close() throws IOException;
}
```
```java
package java.io;

public abstract class Writer implements Appendable, Closeable, Flushable {
    // 注意这个不是缓存，是用来写数据转化的数组对象，WRITE_BUFFER_SIZE大小以内的数据转化时复用
    private char[] writeBuffer;
    private static final int WRITE_BUFFER_SIZE = 1024;
    protected Object lock;
    
    protected Writer() {
        this.lock = this;
    }
    
    protected Writer(Object lock) {
        if (lock == null) {
            throw new NullPointerException();
        }
        this.lock = lock;
    }
    
    // 需要子类实现的写入数据方法，时所有write方法的基础，其他方法都类似
    abstract public void write(char cbuf[], int off, int len) throws IOException;
    
    // 这个方法注意，第一次写入时都会创建writeBuffer
     public void write(int c) throws IOException {
        synchronized (lock) {
            if (writeBuffer == null){
                writeBuffer = new char[WRITE_BUFFER_SIZE];
            }
            writeBuffer[0] = (char) c;
            write(writeBuffer, 0, 1);
        }
    }
    
   // String入参的方法特别注意下，涉及到写缓存的创建，以及为什么要使用writeBuffer，底层使用的char数组传值的
    public void write(String str, int off, int len) throws IOException {
        synchronized (lock) {
            char cbuf[];
            if (len <= WRITE_BUFFER_SIZE) {
                if (writeBuffer == null) {
                    writeBuffer = new char[WRITE_BUFFER_SIZE];
                }
                cbuf = writeBuffer;
            } else {    // Don't permanently allocate very large buffers.
                cbuf = new char[len];
            }
            str.getChars(off, (off + len), cbuf, 0);
            write(cbuf, 0, len);
        }
    }
    
    // 除了write方法外，还提供了append方法，本质还是调用的write方法，但是可以实现链式调用a.append().append()
    public Writer append(CharSequence csq, int start, int end) throws IOException {
        CharSequence cs = (csq == null ? "null" : csq);
        write(cs.subSequence(start, end).toString());
        return this;
    }

    // 和OutputStream方法功能类似
    abstract public void close() throws IOException;
    abstract public void flush() throws IOException;
}
```
### `FileInputStream`&`FileOutputStream`
使用文件作为输入输出源的io操作类，大体方法都和基本差不多，核心代码是基于文件的实现。
```java
public
class FileInputStream extends InputStream
{
	// 文件操作的两个关键变量
	private final FileDescriptor fd;
    private FileChannel channel = null;

	// 只介绍一个构造函数，其他类似
    public FileInputStream(File file) throws FileNotFoundException {
        String name = (file != null ? file.getPath() : null);
        // 安全校验
        SecurityManager security = System.getSecurityManager();
        if (security != null) {
            security.checkRead(name);
        }
        if (name == null) {
            throw new NullPointerException();
        }
        if (file.isInvalid()) {
            throw new FileNotFoundException("Invalid file path");
        }
        fd = new FileDescriptor();
        fd.attach(this);
        path = name;
        
        // 打开文件，open调用的native方法open0()
        open(name);
    }

	// 打开文件的本地方法
	private native void open0(String name) throws FileNotFoundException;

	// file操作的核心方法，读取一个字符
    private native int read0() throws IOException;

    // file操作的核心方法，读取数据存放到数组
    private native int readBytes(byte b[], int off, int len) throws IOException;

	// 调用native方法skip数据
	private native long skip0(long n) throws IOException;
    
    // 调用native方法获取可读数
    private native int available0() throws IOException;
}
```
```java
public class FileOutputStream extends OutputStream
{
	// 构造函数和FileInputStream类
   	public FileOutputStream(File file, boolean append)
        throws FileNotFoundException
    {}
    
    // 读取的主要方法
    private native void writeBytes(byte b[], int off, int len, boolean append)
        throws IOException;

    private native void write(int b, boolean append) throws IOException;

}
```
使用样例如下
```java
Test {
	//文件字节流
     File f = new File("word.txt");//创建文件word.txt

     //输出流FileOutputStream，将数据输出到word.txt
     //不可以在word.txt中直接编辑，除非FileOutputStream被注释了（try-catch是系统自动填充的）
     try {
         FileOutputStream out = new FileOutputStream(f,false);;//创建FileOutputStream对象
         //false文件输出流，在文件末尾不可追加，默认；true文件输出流，在文件末尾可追加
        String s  = "你好啊;";
        byte b[] = s.getBytes();//字符串装换为字节数组

        out.write(b);//将字节数组中的数据写入到文件当中

        out.close();//java在程序结束时自动关闭所有打开的流，但是当使用完留后，显示的关闭是所有打开的流是一个好习惯

    } catch (FileNotFoundException e) {
        // TODO 自动生成的 catch 块
        e.printStackTrace();
    } catch (IOException e) {
        // TODO 自动生成的 catch 块
        e.printStackTrace();
    }


     //输入流FileInputStream，将数据读出来
     FileInputStream in = null ;
     try {
        in = new FileInputStream(f);//输出流读文件

        byte b2[] = new byte[1024];//缓存区1024个字节，有空格的产生

        int len = in.read(b2);//读入缓存区的总字节数
        //S ystem.out.println("文件中的内容是:" +new String(b2));//不去掉空格
        System.out.println("文件中的内容是:" +new String(b2,0,len));//去掉空格

    } catch (FileNotFoundException e) {
        // TODO 自动生成的 catch 块
        e.printStackTrace();
    } catch (IOException e) {
        // TODO 自动生成的 catch 块
        e.printStackTrace();
    }finally {
        if( in != null) {
            try {
                //java在程序结束时自动关闭所有打开的流，但是当使用完留后，显示的关闭是所有打开的流是一个好习惯
                in.close();
            } catch (IOException e) {
                // TODO 自动生成的 catch 块
                e.printStackTrace();
            }
        }
    }
}
```

### `PipeInputStream`&`PipeInputStream`
```java
public class PipedInputStream extends InputStream {
	protected byte buffer[];
	private static final int DEFAULT_PIPE_SIZE = 1024;
	protected static final int PIPE_SIZE = DEFAULT_PIPE_SIZE;

    public PipedInputStream(PipedOutputStream src, int pipeSize)
            throws IOException {
         initPipe(pipeSize);
         connect(src);
    }
    
    public void connect(PipedOutputStream src) throws IOException {
        src.connect(this);
    }
    
    protected synchronized void receive(int b) throws IOException {
        checkStateForReceive();
        writeSide = Thread.currentThread();
        if (in == out)
            awaitSpace();
        if (in < 0) {
            in = 0;
            out = 0;
        }
        buffer[in++] = (byte)(b & 0xFF);
        if (in >= buffer.length) {
            in = 0;
        }
    }

    synchronized void receive(byte b[], int off, int len)  throws IOException {
        checkStateForReceive();
        writeSide = Thread.currentThread();
        int bytesToTransfer = len;
        while (bytesToTransfer > 0) {
            if (in == out)
                awaitSpace();
            int nextTransferAmount = 0;
            if (out < in) {
                nextTransferAmount = buffer.length - in;
            } else if (in < out) {
                if (in == -1) {
                    in = out = 0;
                    nextTransferAmount = buffer.length - in;
                } else {
                    nextTransferAmount = out - in;
                }
            }
            if (nextTransferAmount > bytesToTransfer)
                nextTransferAmount = bytesToTransfer;
            assert(nextTransferAmount > 0);
            System.arraycopy(b, off, buffer, in, nextTransferAmount);
            bytesToTransfer -= nextTransferAmount;
            off += nextTransferAmount;
            in += nextTransferAmount;
            if (in >= buffer.length) {
                in = 0;
            }
        }
    }

    public synchronized int read(byte b[], int off, int len)  throws IOException {
        if (b == null) {
            throw new NullPointerException();
        } else if (off < 0 || len < 0 || len > b.length - off) {
            throw new IndexOutOfBoundsException();
        } else if (len == 0) {
            return 0;
        }

        /* possibly wait on the first character */
        int c = read();
        if (c < 0) {
            return -1;
        }
        b[off] = (byte) c;
        int rlen = 1;
        while ((in >= 0) && (len > 1)) {

            int available;

            if (in > out) {
                available = Math.min((buffer.length - out), (in - out));
            } else {
                available = buffer.length - out;
            }

            // A byte is read beforehand outside the loop
            if (available > (len - 1)) {
                available = len - 1;
            }
            System.arraycopy(buffer, out, b, off + rlen, available);
            out += available;
            rlen += available;
            len -= available;

            if (out >= buffer.length) {
                out = 0;
            }
            if (in == out) {
                /* now empty */
                in = -1;
            }
        }
        return rlen;
    }

    public synchronized int available() throws IOException {
        if(in < 0)
            return 0;
        else if(in == out)
            return buffer.length;
        else if (in > out)
            return in - out;
        else
            return in + buffer.length - out;
    }

    synchronized void receivedLast() {
        closedByWriter = true;
        notifyAll();
    }

    private void awaitSpace() throws IOException {
        while (in == out) {
            checkStateForReceive();

            /* full: kick any waiting readers */
            notifyAll();
            try {
                wait(1000);
            } catch (InterruptedException ex) {
                throw new java.io.InterruptedIOException();
            }
        }
    }


}
```
```java
public class PipedOutputStream extends OutputStream {
	private PipedInputStream sink;

    public PipedOutputStream(PipedInputStream snk)  throws IOException {
        connect(snk);
    }

    public synchronized void connect(PipedInputStream snk) throws IOException {
        if (snk == null) {
            throw new NullPointerException();
        } else if (sink != null || snk.connected) {
            throw new IOException("Already connected");
        }
        sink = snk;
        snk.in = -1;
        snk.out = 0;
        snk.connected = true;
    }

    public void write(byte b[], int off, int len) throws IOException {
        if (sink == null) {
            throw new IOException("Pipe not connected");
        } else if (b == null) {
            throw new NullPointerException();
        } else if ((off < 0) || (off > b.length) || (len < 0) ||
                   ((off + len) > b.length) || ((off + len) < 0)) {
            throw new IndexOutOfBoundsException();
        } else if (len == 0) {
            return;
        }
        sink.receive(b, off, len);
    }

    public synchronized void flush() throws IOException {
        if (sink != null) {
            synchronized (sink) {
                sink.notifyAll();
            }
        }
    }

    public void write(int b)  throws IOException {
        if (sink == null) {
            throw new IOException("Pipe not connected");
        }
        sink.receive(b);
    }

    public void close()  throws IOException {
        if (sink != null) {
            sink.receivedLast();
        }
    }
}
```
使用样例
```java
public class MyProducer extends Thread {

	private PipedOutputStream outputStream;

	public MyProducer(PipedOutputStream outputStream) {
		this.outputStream = outputStream;
	}
 
	@Override
	public void run() {
		while (true) {
			try {
				for (int i = 0; i < 5; i++) {
					outputStream.write(i);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	} 
}

public class MyConsumer extends Thread {
	
	private PipedInputStream inputStream;
	 
	public MyConsumer(PipedInputStream inputStream) {
		this.inputStream = inputStream;
	}
 
	@Override
	public void run() {
		while (true) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			try {
				int count = inputStream.available();
				if (count > 0) {
					System.out.println("rest product count: " + count);
					System.out.println("get product: " + inputStream.read());
				}
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}
}


public class PipeTest {
	
	public static void main(String[] args) {
		 
		PipedOutputStream pos = new PipedOutputStream();
		PipedInputStream pis = new PipedInputStream();
		try {
			pis.connect(pos);
		} catch (IOException e) {
			e.printStackTrace();
		}
		new MyProducer(pos).start();
		new MyConsumer(pis).start();
	}
}
```
### `FileReader`&`Filewriter`
这两个类很有意思，实现上做了很多设计。FileReader本身这是构建了FileInputStream传入，剩下的都在InputStreamReader类中。而InputStreamReader又是一个装饰类，完成字符字节流的转化工作。实际上通过StreamDecoder类实现功能，StreamDecoder类又将字符解码和io拆分开，只做字符解码，最终的io动作都是传入的InputStream来完成，而FileReader就是通过构造的FileInputStream来完成io操作的。FileWriter也是类似设计的。

```java
public class FileReader extends InputStreamReader {
	// 只有三个构造方法，都类似如下
    public FileReader(File file) throws FileNotFoundException {
        super(new FileInputStream(file));
    }
}
```
```java
public class InputStreamReader extends Reader {
    private final StreamDecoder sd;

    public int read(char cbuf[], int offset, int length) throws IOException {
        return sd.read(cbuf, offset, length);
    }
}
```
```java
public class StreamDecoder extends Reader {
    private Charset cs;
    private CharsetDecoder decoder;
    private ByteBuffer bb;
    // inputstream处理对象
    private InputStream in;

    StreamDecoder(InputStream var1, Object var2, CharsetDecoder var3) {
        super(var2);
        this.isOpen = true;
        this.haveLeftoverChar = false;
        this.cs = var3.charset();
        this.decoder = var3;
        if (this.ch == null) {
            this.in = var1;
            this.ch = null;
            this.bb = ByteBuffer.allocate(8192);
        }

        this.bb.flip();
    }

    public int read(char[] var1, int var2, int var3) throws IOException {
        int var4 = var2;
        int var5 = var3;
        synchronized(this.lock) {
            this.ensureOpen();
            if (var4 >= 0 && var4 <= var1.length && var5 >= 0 && var4 + var5 <= var1.length && var4 + var5 >= 0) {
                if (var5 == 0) {
                    return 0;
                } else {
                    byte var7 = 0;
                    if (this.haveLeftoverChar) {
                        var1[var4] = this.leftoverChar;
                        ++var4;
                        --var5;
                        this.haveLeftoverChar = false;
                        var7 = 1;
                        if (var5 == 0 || !this.implReady()) {
                            return var7;
                        }
                    }

                    if (var5 == 1) {
                        int var8 = this.read0();
                        if (var8 == -1) {
                            return var7 == 0 ? -1 : var7;
                        } else {
                            var1[var4] = (char)var8;
                            return var7 + 1;
                        }
                    } else {
                        return var7 + this.implRead(var1, var4, var4 + var5);
                    }
                }
            } else {
                throw new IndexOutOfBoundsException();
            }
        }
    }

    int implRead(char[] var1, int var2, int var3) throws IOException {
        assert var3 - var2 > 1;

        CharBuffer var4 = CharBuffer.wrap(var1, var2, var3 - var2);
        if (var4.position() != 0) {
            var4 = var4.slice();
        }

        boolean var5 = false;

        while(true) {
            CoderResult var6 = this.decoder.decode(this.bb, var4, var5);
            if (var6.isUnderflow()) {
                if (var5 || !var4.hasRemaining() || var4.position() > 0 && !this.inReady()) {
                    break;
                }

                int var7 = this.readBytes();
                if (var7 < 0) {
                    var5 = true;
                    if (var4.position() == 0 && !this.bb.hasRemaining()) {
                        break;
                    }

                    this.decoder.reset();
                }
            } else {
                if (var6.isOverflow()) {
                    assert var4.position() > 0;
                    break;
                }

                var6.throwException();
            }
        }

        if (var5) {
            this.decoder.reset();
        }

        if (var4.position() == 0) {
            if (var5) {
                return -1;
            }

            assert false;
        }

        return var4.position();
    }

    private int readBytes() throws IOException {
        this.bb.compact();

        int var1;
        try {
            int var2;
            if (this.ch != null) {
                var1 = this.ch.read(this.bb);
                if (var1 < 0) {
                    var2 = var1;
                    return var2;
                }
            } else {
                var1 = this.bb.limit();
                var2 = this.bb.position();

                assert var2 <= var1;

                int var3 = var2 <= var1 ? var1 - var2 : 0;

                assert var3 > 0;

                int var4 = this.in.read(this.bb.array(), this.bb.arrayOffset() + var2, var3);
                if (var4 < 0) {
                    int var5 = var4;
                    return var5;
                }

                if (var4 == 0) {
                    throw new IOException("Underlying input stream returned zero bytes");
                }

                assert var4 <= var3 : "n = " + var4 + ", rem = " + var3;

                this.bb.position(var2 + var4);
            }
        } finally {
            this.bb.flip();
        }

        var1 = this.bb.remaining();

        assert var1 != 0 : var1;

        return var1;
    }
}
```


### 参考
![IO流结构图](https://www.pianshen.com/images/420/3624b0da89209bc848a38b161b3aaefc.JPEG)
![IO类图大全](https://pic1.zhimg.com/v2-adf9568db6289098b6db2a469b44aa5d_r.jpg?source=1940ef5c)
[为什么IO很复杂](https://baijiahao.baidu.com/s?id=1668620974536611727&wfr=spider&for=pc)
[javaIO分析](https://www.jianshu.com/p/613ee60e08b4)
[IO类思维导图](https://pic1.zhimg.com/v2-ae21aba56d1e4f8ebedcc748d20fe6bf_r.jpg?source=1940ef5c)

## IO的同步和异步
### BIO
同步IO处理
### NIO
异步IO处理
### AIO
纯异步IO处理


