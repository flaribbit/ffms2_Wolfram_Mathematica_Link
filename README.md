# ffms2_Wolfram_Mathematica_Link

A library for Wolfram Mathematica (version 11+,mine 12) to read video file via ffms2 library.

## How to compile

Open CMD, then run `make`

## Sample code:

First, declaring functions: 

```Mathematica
Module[{lib,ffms2,ffms3,ffms4},
lib=ExpandFileName@"ffmslink.dll";
LibraryLoad@ExpandFileName@"ffms2.dll";
LibraryFunctionLoad[lib,"Init",{},"Void"][];
ffms2=LibraryFunctionLoad[lib,"OpenVideoSource",{String},Integer];
ffms3=LibraryFunctionLoad[lib,"CloseVideoSource",{Integer},"Void"];
ffms4=LibraryFunctionLoad[lib,"GetFrame",{Integer,Integer},Image];
FFMSOpen[file_]:=Block[{p},p=ffms2[file];If[p>0,video$[p],$Failed]];
GetFrame[n_]@video$@p_:=If[p===$Failed,$Failed,ffms4[p,n]];
Destroy@video$@p_:=ffms3[p];]
```

Then:

```Mathematica
video = FFMSOpen["test.flv"]
video // GetFrame[0]
video // Destroy
```
