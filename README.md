# oFHRTF

[![oFHRTF](http://img.youtube.com/vi/dod9TOJ_nJo/0.jpg)](https://www.youtube.com/watch?v=dod9TOJ_nJo "oFHRTF")


## Environment

```
macOS Cataline
openFrameworks v0.11.0
```

## Setup

### openFrameworks

1. download openFrameworks0.11.0 from [website](https://openframeworks.cc/download/) according to your environment.

2. unzip openFrameworks0.11.0 and move it to your favorite folder.

3. enter the following commands.

```sh
cd openFrameworksFolder/apps/myApps/
git clone https://github.com/mynkit/oFHRTF.git
cd oFHRTF
```

### Database of HRTF

以下の[名古屋大学公開のHRTFデータベース](http://www.sp.m.is.nagoya-u.ac.jp/HRTF/database.html)をダウンロードし、`./bin/data`配下に`hrtfs`ディレクトリを格納してください

Download the following HRTF database published by [Nagoya University](http://www.sp.m.is.nagoya-u.ac.jp/HRTF/database.html) and Store `hrtfs/` under `./bin/data`.


http://www.sp.m.is.nagoya-u.ac.jp/HRTF/archive/data02.tgz

## How to Use

1. `connect Headphones or Audio Interface, or HOWLING might be caused!!`

2. open `oFHRTF.xcodeproj` and Run app.
