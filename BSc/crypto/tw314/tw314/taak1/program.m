function varargout = program(varargin)
% PROGRAM M-file for program.fig
%      PROGRAM, by itself, creates a new PROGRAM or raises the existing
%      singleton*.
%
%      H = PROGRAM returns the handle to a new PROGRAM or the handle to
%      the existing singleton*.
%
%      PROGRAM('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in PROGRAM.M with the given input arguments.
%
%      PROGRAM('Property','Value',...) creates a new PROGRAM or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before program_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to program_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help program

% Last Modified by GUIDE v2.5 03-Apr-2005 13:39:09

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @program_OpeningFcn, ...
                   'gui_OutputFcn',  @program_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before program is made visible.
function program_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no t_output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to program (see VARARGIN)

% Choose default command line t_output for program
handles.t_output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes program wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = program_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning t_output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line t_output from handles structure
varargout{1} = handles.t_output;



function input_Callback(hObject, eventdata, handles)
% hObject    handle to input (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of input as text
%        str2double(get(hObject,'String')) returns contents of input as a double


% --- Executes during object creation, after setting all properties.
function input_CreateFcn(hObject, eventdata, handles)
% hObject    handle to input (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in b_dec.
function b_dec_Callback(hObject, eventdata, handles)
% hObject    handle to b_dec (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

s = str2num(get(handles.s_length,'String'));
kripto = get(handles.input,'String')
teks = decode(kripto,s)
set(handles.out,'String',teks);

% --- Executes on button press in b_enc.
function b_enc_Callback(hObject, eventdata, handles)
% hObject    handle to b_enc (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

s = str2num(get(handles.s_length,'String'));
teks = get(handles.input,'String');
kripto = encode(teks,s);
set(handles.out,'String',kripto);

function s_length_Callback(hObject, eventdata, handles)
% hObject    handle to s_length (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of s_length as text
%        str2double(get(hObject,'String')) returns contents of s_length as a double


% --- Executes during object creation, after setting all properties.
function s_length_CreateFcn(hObject, eventdata, handles)
% hObject    handle to s_length (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end





function out_Callback(hObject, eventdata, handles)
% hObject    handle to out (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of out as text
%        str2double(get(hObject,'String')) returns contents of out as a double


% --- Executes during object creation, after setting all properties.
function out_CreateFcn(hObject, eventdata, handles)
% hObject    handle to out (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


