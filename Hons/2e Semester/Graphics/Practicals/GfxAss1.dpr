program GfxAss1;

uses
  Windows, Messages,
  DirectXGraphics,
  DXGfx,
  DXFont,
  DXTimer;

var
  handle  : HWND;
  Font    : TFontHandle;

  Surface : IDirect3DSurface8;
  srfLock : TD3DLocked_Rect;

procedure PutPixel(x, y, c : cardinal);
{ on entry:  x = eax,   y = edx,   color = ecx }
asm
   push esi

   mov esi,srfLock.pBits
   shl eax,2
   add esi,eax                           // description similar to PutPixel8
   mov eax,[srfLock.Pitch]
   mul edx
   add esi,eax

   mov ds:[esi],ecx

   pop esi
   ret
end;

function GetPixel(x, y : cardinal) : byte;
{ on entry:  x = eax,   y = edx}
asm
   push esi

   mov esi,srfLock.pBits
   shl eax,2
   add esi,eax                           // description similar to PutPixel8
   mov eax,[srfLock.Pitch]
   mul edx
   add esi,eax

   mov eax,ds:[esi]

   pop esi
   ret
end;

function col(v : cardinal) : cardinal;
begin
  Result := v shl 24 + v shl 16 + v shl 8 + v;
end;

function GetValue(b1, b2 : byte; range : integer) : integer;
begin
  Result := (b1 + b2) div 2 + random(range) - range div 2;
  if Result < 0 then Result := 0;
  if Result > 255 then Result := 255;
  Result := col(Result);
end;

function GetMidValue(x1, y1, x2, y2, range : integer) : integer;
begin
  Result := GetPixel(x1, y1) + GetPixel(x1, y2) + GetPixel(x2, y1) + GetPixel(x2, y2);
  Result := Result div 4;
  Result := Result + random(range) - range div 2;
  if Result < 0 then Result := 0;
  if Result > 255 then Result := 255;
  Result := col(Result);
end;

procedure MidPointRecurse(x1, y1, x2, y2 : integer);
begin
  if x1 = x2 - 1 then exit;
  if GetPixel((x1 + x2) div 2, y1) = 0 then PutPixel((x1 + x2) div 2, y1, GetValue(GetPixel(x1, y1), GetPixel(x2, y1), x2 - x1));
  if GetPixel((x1 + x2) div 2, y2) = 0 then PutPixel((x1 + x2) div 2, y2, GetValue(GetPixel(x1, y2), GetPixel(x2, y2), x2 - x1));
  if GetPixel(x1, (y1 + y2) div 2) = 0 then PutPixel(x1, (y1 + y2) div 2, GetValue(GetPixel(x1, y1), GetPixel(x1, y2), x2 - x1));
  if GetPixel(x2, (y1 + y2) div 2) = 0 then PutPixel(x2, (y1 + y2) div 2, GetValue(GetPixel(x2, y1), GetPixel(x2, y2), x2 - x1));
  PutPixel((x1 + x2) div 2, (y1 + y2) div 2, GetMidValue(x1, y1, x2, y2, x2 - x1));
  MidPointRecurse(x1, y1, (x1 + x2) div 2, (y1 + y2) div 2);
  MidPointRecurse((x1 + x2) div 2, y1, x2, (y1 + y2) div 2);
  MidPointRecurse(x1, (y1 + y2) div 2, (x1 + x2) div 2, y2);
  MidPointRecurse((x1 + x2) div 2, (y1 + y2) div 2, x2, y2);
end;

procedure MidPointDisplace;
begin
  D3DDev8.Clear(0, NIL, D3DCLEAR_TARGET, $00000000, 1.0, 0);
  D3DDev8.Clear(0, NIL, D3DCLEAR_ZBUFFER, $0, 1.0, 0);
  D3DDev8.BeginScene;
  if Surface.LockRect(srfLock, nil, 0) <> D3D_OK then halt(21);

  PutPixel(0, 0, col(random(256)));
  PutPixel(256, 0, col(random(256)));
  PutPixel(0, 256, col(random(256)));
  PutPixel(256, 256, col(random(256)));
  MidPointRecurse(0, 0, 256, 256);

  Surface.UnlockRect;
  D3DDev8.EndScene;
  D3DDev8.Present(NIL, NIL, 0, NIL);
end;

function WndProc(HWnd: HWND; Msg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
// standard minimal windows callback function
begin
  Result := 0;
  case Msg of
    WM_DESTROY : begin
                   ShutDown := true;
                   PostQuitMessage(0);
                   Result := 0;
                 end;
    WM_KEYDOWN : begin
                   case wParam of
                     VK_ESCAPE : begin ShutDown := true; PostQuitMessage(0); end;
                     VK_SPACE  : begin MidPointDisplace; end;
                   end;
                 end;
  else
    Result := DefWindowProc(HWnd, Msg, WParam, LParam);
  end;
end;

begin
  WndClass.lpfnWndProc := @WndProc;
  handle := InitWindow(windowed, 100, 100, 265, 284, 'Mid Point Displacement Terrain');
  if not InitGfx(handle, windowed, 256, 256, D3DFMT_X8R8G8B8, 1, true, true, false, true) then begin
    MessageBox(handle, 'Could not initialize 3D device', 'Error!', mb_OK);
    halt(21);
  end;

  Font := TFontHandle.Create;
  Font.InitializeFont(D3DDev8, HFONT(GetStockObject(System_Font)), $ffffffff);

  if D3DDev8.GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, Surface) <> D3D_OK then halt(21);

  Randomize;
  ShutDown := false;

  MidPointDisplace;

  while not ShutDown do begin
    MaintainWindow(handle);
  end;

  Surface := nil;
  Font.Destroy;
  DeinitGfx;
end.
