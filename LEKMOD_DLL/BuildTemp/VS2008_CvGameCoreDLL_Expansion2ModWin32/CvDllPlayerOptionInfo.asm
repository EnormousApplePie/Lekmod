; Listing generated by Microsoft (R) Optimizing Compiler Version 15.00.30729.01 

	TITLE	c:\Users\EnormousApplePie\Documents\GitHub\Lekmod\LEKMOD_DLL\CvGameCoreDLL_Expansion2\CvDllPlayerOptionInfo.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB MSVCRT
INCLUDELIB OLDNAMES

_DATA	SEGMENT
_CIV5_XP2_DLL_VERSION DD FLAT:$SG217703
_DATA	ENDS
CONST	SEGMENT
$SG217703 DB	'1.0.0', 00H
	ORG $+2
_guidICvUnknown DD 0d89ba82fH
	DW	09fa3H
	DW	04696H
	DB	0b3H
	DB	0f4H
	DB	052H
	DB	0bdH
	DB	0b1H
	DB	01H
	DB	0cfH
	DB	0b2H
_guidICvPlayerOptionInfo1 DD 08c315274H
	DW	0e9a7H
	DW	04210H
	DB	09bH
	DB	02eH
	DB	0aH
	DB	0f1H
	DB	03cH
	DB	043H
	DB	0c0H
	DB	0b6H
CONST	ENDS
PUBLIC	??_7ICvUnknown@@6B@				; ICvUnknown::`vftable'
PUBLIC	??0ICvUnknown@@QAE@XZ				; ICvUnknown::ICvUnknown
PUBLIC	??_R4ICvUnknown@@6B@				; ICvUnknown::`RTTI Complete Object Locator'
PUBLIC	??_R0?AVICvUnknown@@@8				; ICvUnknown `RTTI Type Descriptor'
PUBLIC	??_R3ICvUnknown@@8				; ICvUnknown::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R2ICvUnknown@@8				; ICvUnknown::`RTTI Base Class Array'
PUBLIC	??_R1A@?0A@EA@ICvUnknown@@8			; ICvUnknown::`RTTI Base Class Descriptor at (0,-1,0,64)'
EXTRN	??_7type_info@@6B@:QWORD			; type_info::`vftable'
EXTRN	__purecall:PROC
;	COMDAT ??_R1A@?0A@EA@ICvUnknown@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@ICvUnknown@@8 DD FLAT:??_R0?AVICvUnknown@@@8 ; ICvUnknown::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	00H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	FLAT:??_R3ICvUnknown@@8
rdata$r	ENDS
;	COMDAT ??_R2ICvUnknown@@8
rdata$r	SEGMENT
??_R2ICvUnknown@@8 DD FLAT:??_R1A@?0A@EA@ICvUnknown@@8	; ICvUnknown::`RTTI Base Class Array'
rdata$r	ENDS
;	COMDAT ??_R3ICvUnknown@@8
rdata$r	SEGMENT
??_R3ICvUnknown@@8 DD 00H				; ICvUnknown::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	01H
	DD	FLAT:??_R2ICvUnknown@@8
rdata$r	ENDS
;	COMDAT ??_R0?AVICvUnknown@@@8
_DATA	SEGMENT
??_R0?AVICvUnknown@@@8 DD FLAT:??_7type_info@@6B@	; ICvUnknown `RTTI Type Descriptor'
	DD	00H
	DB	'.?AVICvUnknown@@', 00H
_DATA	ENDS
;	COMDAT ??_R4ICvUnknown@@6B@
rdata$r	SEGMENT
??_R4ICvUnknown@@6B@ DD 00H				; ICvUnknown::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	FLAT:??_R0?AVICvUnknown@@@8
	DD	FLAT:??_R3ICvUnknown@@8
rdata$r	ENDS
;	COMDAT ??_7ICvUnknown@@6B@
CONST	SEGMENT
??_7ICvUnknown@@6B@ DD FLAT:??_R4ICvUnknown@@6B@	; ICvUnknown::`vftable'
	DD	FLAT:__purecall
	DD	FLAT:__purecall
; Function compile flags: /Ogtpy
CONST	ENDS
;	COMDAT ??0ICvUnknown@@QAE@XZ
_TEXT	SEGMENT
??0ICvUnknown@@QAE@XZ PROC				; ICvUnknown::ICvUnknown, COMDAT
; _this$ = ecx
	mov	eax, ecx
	mov	DWORD PTR [eax], OFFSET ??_7ICvUnknown@@6B@
	ret	0
??0ICvUnknown@@QAE@XZ ENDP				; ICvUnknown::ICvUnknown
_TEXT	ENDS
PUBLIC	??_7CvDllPlayerOptionInfo@@6B@			; CvDllPlayerOptionInfo::`vftable'
PUBLIC	??1CvDllPlayerOptionInfo@@QAE@XZ		; CvDllPlayerOptionInfo::~CvDllPlayerOptionInfo
PUBLIC	??_R4CvDllPlayerOptionInfo@@6B@			; CvDllPlayerOptionInfo::`RTTI Complete Object Locator'
PUBLIC	??_R0?AVCvDllPlayerOptionInfo@@@8		; CvDllPlayerOptionInfo `RTTI Type Descriptor'
PUBLIC	??_R3CvDllPlayerOptionInfo@@8			; CvDllPlayerOptionInfo::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R2CvDllPlayerOptionInfo@@8			; CvDllPlayerOptionInfo::`RTTI Base Class Array'
PUBLIC	??_R1A@?0A@EA@CvDllPlayerOptionInfo@@8		; CvDllPlayerOptionInfo::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R1A@?0A@EA@ICvPlayerOptionInfo1@@8		; ICvPlayerOptionInfo1::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R0?AVICvPlayerOptionInfo1@@@8		; ICvPlayerOptionInfo1 `RTTI Type Descriptor'
PUBLIC	??_R3ICvPlayerOptionInfo1@@8			; ICvPlayerOptionInfo1::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R2ICvPlayerOptionInfo1@@8			; ICvPlayerOptionInfo1::`RTTI Base Class Array'
PUBLIC	?QueryInterface@CvDllPlayerOptionInfo@@UAGPAXU_GUID@@@Z ; CvDllPlayerOptionInfo::QueryInterface
PUBLIC	?Destroy@CvDllPlayerOptionInfo@@EAGXXZ		; CvDllPlayerOptionInfo::Destroy
PUBLIC	?GetDefault@CvDllPlayerOptionInfo@@UAG_NXZ	; CvDllPlayerOptionInfo::GetDefault
;	COMDAT ??_R2ICvPlayerOptionInfo1@@8
; File c:\users\enormousapplepie\documents\github\lekmod\lekmod_dll\cvgamecoredll_expansion2\cvdllplayeroptioninfo.cpp
rdata$r	SEGMENT
??_R2ICvPlayerOptionInfo1@@8 DD FLAT:??_R1A@?0A@EA@ICvPlayerOptionInfo1@@8 ; ICvPlayerOptionInfo1::`RTTI Base Class Array'
	DD	FLAT:??_R1A@?0A@EA@ICvUnknown@@8
rdata$r	ENDS
;	COMDAT ??_R3ICvPlayerOptionInfo1@@8
rdata$r	SEGMENT
??_R3ICvPlayerOptionInfo1@@8 DD 00H			; ICvPlayerOptionInfo1::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	02H
	DD	FLAT:??_R2ICvPlayerOptionInfo1@@8
rdata$r	ENDS
;	COMDAT ??_R0?AVICvPlayerOptionInfo1@@@8
_DATA	SEGMENT
??_R0?AVICvPlayerOptionInfo1@@@8 DD FLAT:??_7type_info@@6B@ ; ICvPlayerOptionInfo1 `RTTI Type Descriptor'
	DD	00H
	DB	'.?AVICvPlayerOptionInfo1@@', 00H
_DATA	ENDS
;	COMDAT ??_R1A@?0A@EA@ICvPlayerOptionInfo1@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@ICvPlayerOptionInfo1@@8 DD FLAT:??_R0?AVICvPlayerOptionInfo1@@@8 ; ICvPlayerOptionInfo1::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	01H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	FLAT:??_R3ICvPlayerOptionInfo1@@8
rdata$r	ENDS
;	COMDAT ??_R1A@?0A@EA@CvDllPlayerOptionInfo@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@CvDllPlayerOptionInfo@@8 DD FLAT:??_R0?AVCvDllPlayerOptionInfo@@@8 ; CvDllPlayerOptionInfo::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	02H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	FLAT:??_R3CvDllPlayerOptionInfo@@8
rdata$r	ENDS
;	COMDAT ??_R2CvDllPlayerOptionInfo@@8
rdata$r	SEGMENT
??_R2CvDllPlayerOptionInfo@@8 DD FLAT:??_R1A@?0A@EA@CvDllPlayerOptionInfo@@8 ; CvDllPlayerOptionInfo::`RTTI Base Class Array'
	DD	FLAT:??_R1A@?0A@EA@ICvPlayerOptionInfo1@@8
	DD	FLAT:??_R1A@?0A@EA@ICvUnknown@@8
rdata$r	ENDS
;	COMDAT ??_R3CvDllPlayerOptionInfo@@8
rdata$r	SEGMENT
??_R3CvDllPlayerOptionInfo@@8 DD 00H			; CvDllPlayerOptionInfo::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	03H
	DD	FLAT:??_R2CvDllPlayerOptionInfo@@8
rdata$r	ENDS
;	COMDAT ??_R0?AVCvDllPlayerOptionInfo@@@8
_DATA	SEGMENT
??_R0?AVCvDllPlayerOptionInfo@@@8 DD FLAT:??_7type_info@@6B@ ; CvDllPlayerOptionInfo `RTTI Type Descriptor'
	DD	00H
	DB	'.?AVCvDllPlayerOptionInfo@@', 00H
_DATA	ENDS
;	COMDAT ??_R4CvDllPlayerOptionInfo@@6B@
rdata$r	SEGMENT
??_R4CvDllPlayerOptionInfo@@6B@ DD 00H			; CvDllPlayerOptionInfo::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	FLAT:??_R0?AVCvDllPlayerOptionInfo@@@8
	DD	FLAT:??_R3CvDllPlayerOptionInfo@@8
rdata$r	ENDS
;	COMDAT ??_7CvDllPlayerOptionInfo@@6B@
CONST	SEGMENT
??_7CvDllPlayerOptionInfo@@6B@ DD FLAT:??_R4CvDllPlayerOptionInfo@@6B@ ; CvDllPlayerOptionInfo::`vftable'
	DD	FLAT:?QueryInterface@CvDllPlayerOptionInfo@@UAGPAXU_GUID@@@Z
	DD	FLAT:?Destroy@CvDllPlayerOptionInfo@@EAGXXZ
	DD	FLAT:?GetDefault@CvDllPlayerOptionInfo@@UAG_NXZ
; Function compile flags: /Ogtpy
CONST	ENDS
;	COMDAT ??1CvDllPlayerOptionInfo@@QAE@XZ
_TEXT	SEGMENT
??1CvDllPlayerOptionInfo@@QAE@XZ PROC			; CvDllPlayerOptionInfo::~CvDllPlayerOptionInfo, COMDAT
; _this$ = ecx

; 24   : {

	mov	DWORD PTR [ecx], OFFSET ??_7CvDllPlayerOptionInfo@@6B@

; 25   : }

	ret	0
??1CvDllPlayerOptionInfo@@QAE@XZ ENDP			; CvDllPlayerOptionInfo::~CvDllPlayerOptionInfo
_TEXT	ENDS
PUBLIC	?IncrementReference@CvDllPlayerOptionInfo@@QAEIXZ ; CvDllPlayerOptionInfo::IncrementReference
; Function compile flags: /Ogtpy
;	COMDAT ?IncrementReference@CvDllPlayerOptionInfo@@QAEIXZ
_TEXT	SEGMENT
?IncrementReference@CvDllPlayerOptionInfo@@QAEIXZ PROC	; CvDllPlayerOptionInfo::IncrementReference, COMDAT
; _this$ = ecx

; 41   : 	++m_uiRefCount;

	inc	DWORD PTR [ecx+4]

; 42   : 	return m_uiRefCount;

	mov	eax, DWORD PTR [ecx+4]

; 43   : }

	ret	0
?IncrementReference@CvDllPlayerOptionInfo@@QAEIXZ ENDP	; CvDllPlayerOptionInfo::IncrementReference
_TEXT	ENDS
PUBLIC	?GetReferenceCount@CvDllPlayerOptionInfo@@QAEIXZ ; CvDllPlayerOptionInfo::GetReferenceCount
; Function compile flags: /Ogtpy
;	COMDAT ?GetReferenceCount@CvDllPlayerOptionInfo@@QAEIXZ
_TEXT	SEGMENT
?GetReferenceCount@CvDllPlayerOptionInfo@@QAEIXZ PROC	; CvDllPlayerOptionInfo::GetReferenceCount, COMDAT
; _this$ = ecx

; 61   : 	return m_uiRefCount;

	mov	eax, DWORD PTR [ecx+4]

; 62   : }

	ret	0
?GetReferenceCount@CvDllPlayerOptionInfo@@QAEIXZ ENDP	; CvDllPlayerOptionInfo::GetReferenceCount
_TEXT	ENDS
PUBLIC	??3CvDllPlayerOptionInfo@@SAXPAX@Z		; CvDllPlayerOptionInfo::operator delete
EXTRN	?Free@CvDllGameContext@@SAXPAX@Z:PROC		; CvDllGameContext::Free
; Function compile flags: /Ogtpy
;	COMDAT ??3CvDllPlayerOptionInfo@@SAXPAX@Z
_TEXT	SEGMENT
_p$ = 8							; size = 4
??3CvDllPlayerOptionInfo@@SAXPAX@Z PROC			; CvDllPlayerOptionInfo::operator delete, COMDAT

; 71   : 	CvDllGameContext::Free(p);

	jmp	?Free@CvDllGameContext@@SAXPAX@Z	; CvDllGameContext::Free
??3CvDllPlayerOptionInfo@@SAXPAX@Z ENDP			; CvDllPlayerOptionInfo::operator delete
_TEXT	ENDS
PUBLIC	??2CvDllPlayerOptionInfo@@SAPAXI@Z		; CvDllPlayerOptionInfo::operator new
EXTRN	?Allocate@CvDllGameContext@@SAPAXI@Z:PROC	; CvDllGameContext::Allocate
; Function compile flags: /Ogtpy
;	COMDAT ??2CvDllPlayerOptionInfo@@SAPAXI@Z
_TEXT	SEGMENT
_bytes$ = 8						; size = 4
??2CvDllPlayerOptionInfo@@SAPAXI@Z PROC			; CvDllPlayerOptionInfo::operator new, COMDAT

; 76   : 	return CvDllGameContext::Allocate(bytes);

	jmp	?Allocate@CvDllGameContext@@SAPAXI@Z	; CvDllGameContext::Allocate
??2CvDllPlayerOptionInfo@@SAPAXI@Z ENDP			; CvDllPlayerOptionInfo::operator new
_TEXT	ENDS
PUBLIC	?GetInstance@CvDllPlayerOptionInfo@@QAEPAVCvPlayerOptionInfo@@XZ ; CvDllPlayerOptionInfo::GetInstance
; Function compile flags: /Ogtpy
;	COMDAT ?GetInstance@CvDllPlayerOptionInfo@@QAEPAVCvPlayerOptionInfo@@XZ
_TEXT	SEGMENT
?GetInstance@CvDllPlayerOptionInfo@@QAEPAVCvPlayerOptionInfo@@XZ PROC ; CvDllPlayerOptionInfo::GetInstance, COMDAT
; _this$ = ecx

; 81   : 	return m_pPlayerOptionInfo;

	mov	eax, DWORD PTR [ecx+8]

; 82   : }

	ret	0
?GetInstance@CvDllPlayerOptionInfo@@QAEPAVCvPlayerOptionInfo@@XZ ENDP ; CvDllPlayerOptionInfo::GetInstance
_TEXT	ENDS
EXTRN	?getDefault@CvPlayerOptionInfo@@QBE_NXZ:PROC	; CvPlayerOptionInfo::getDefault
; Function compile flags: /Ogtpy
;	COMDAT ?GetDefault@CvDllPlayerOptionInfo@@UAG_NXZ
_TEXT	SEGMENT
_this$ = 8						; size = 4
?GetDefault@CvDllPlayerOptionInfo@@UAG_NXZ PROC		; CvDllPlayerOptionInfo::GetDefault, COMDAT

; 86   : 	return m_pPlayerOptionInfo->getDefault();

	mov	eax, DWORD PTR _this$[esp-4]
	mov	ecx, DWORD PTR [eax+8]
	call	?getDefault@CvPlayerOptionInfo@@QBE_NXZ	; CvPlayerOptionInfo::getDefault

; 87   : }

	ret	4
?GetDefault@CvDllPlayerOptionInfo@@UAG_NXZ ENDP		; CvDllPlayerOptionInfo::GetDefault
_TEXT	ENDS
PUBLIC	_IsEqualGUID
; Function compile flags: /Ogtpy
; File c:\program files\microsoft sdks\windows\v6.0a\include\guiddef.h
;	COMDAT _IsEqualGUID
_TEXT	SEGMENT
_rguid1$ = 8						; size = 4
_rguid2$ = 12						; size = 4
_IsEqualGUID PROC					; COMDAT

; 161  :     return !memcmp(&rguid1, &rguid2, sizeof(GUID));

	mov	ecx, DWORD PTR _rguid2$[esp-4]
	mov	edx, DWORD PTR _rguid1$[esp-4]
	mov	eax, 16					; 00000010H
	push	esi
	npad	2
$LL4@IsEqualGUI:
	mov	esi, DWORD PTR [edx]
	cmp	esi, DWORD PTR [ecx]
	jne	SHORT $LN5@IsEqualGUI
	sub	eax, 4
	add	ecx, 4
	add	edx, 4
	cmp	eax, 4
	jae	SHORT $LL4@IsEqualGUI
	xor	eax, eax
	xor	edx, edx
	test	eax, eax
	sete	dl
	pop	esi
	mov	eax, edx

; 162  : }

	ret	0
$LN5@IsEqualGUI:

; 161  :     return !memcmp(&rguid1, &rguid2, sizeof(GUID));

	movzx	eax, BYTE PTR [edx]
	movzx	esi, BYTE PTR [ecx]
	sub	eax, esi
	jne	SHORT $LN7@IsEqualGUI
	movzx	eax, BYTE PTR [edx+1]
	movzx	esi, BYTE PTR [ecx+1]
	sub	eax, esi
	jne	SHORT $LN7@IsEqualGUI
	movzx	eax, BYTE PTR [edx+2]
	movzx	esi, BYTE PTR [ecx+2]
	sub	eax, esi
	jne	SHORT $LN7@IsEqualGUI
	movzx	eax, BYTE PTR [edx+3]
	movzx	ecx, BYTE PTR [ecx+3]
	sub	eax, ecx
$LN7@IsEqualGUI:
	sar	eax, 31					; 0000001fH
	or	eax, 1
	xor	edx, edx
	test	eax, eax
	sete	dl
	pop	esi
	mov	eax, edx

; 162  : }

	ret	0
_IsEqualGUID ENDP
_TEXT	ENDS
PUBLIC	_==
; Function compile flags: /Ogtpy
;	COMDAT _==
_TEXT	SEGMENT
_guidOne$ = 8						; size = 4
_guidOther$ = 12					; size = 4
_==	PROC						; COMDAT

; 194  :     return IsEqualGUID(guidOne,guidOther);

	jmp	_IsEqualGUID
_==	ENDP
_TEXT	ENDS
PUBLIC	?GetInterfaceId@ICvUnknown@@SG?AU_GUID@@XZ	; ICvUnknown::GetInterfaceId
; Function compile flags: /Ogtpy
; File c:\users\enormousapplepie\documents\github\lekmod\lekmod_dll\cvgamecoredllutil\include\cvdllinterfaces.h
;	COMDAT ?GetInterfaceId@ICvUnknown@@SG?AU_GUID@@XZ
_TEXT	SEGMENT
$T219026 = 8						; size = 4
?GetInterfaceId@ICvUnknown@@SG?AU_GUID@@XZ PROC		; ICvUnknown::GetInterfaceId, COMDAT

; 306  : 	static GUID DLLCALL GetInterfaceId(){ return guidICvUnknown; }

	mov	eax, DWORD PTR $T219026[esp-4]
	mov	ecx, DWORD PTR _guidICvUnknown
	mov	edx, DWORD PTR _guidICvUnknown+4
	mov	DWORD PTR [eax], ecx
	mov	ecx, DWORD PTR _guidICvUnknown+8
	mov	DWORD PTR [eax+4], edx
	mov	edx, DWORD PTR _guidICvUnknown+12
	mov	DWORD PTR [eax+8], ecx
	mov	DWORD PTR [eax+12], edx
	ret	4
?GetInterfaceId@ICvUnknown@@SG?AU_GUID@@XZ ENDP		; ICvUnknown::GetInterfaceId
_TEXT	ENDS
PUBLIC	?GetInterfaceId@ICvPlayerOptionInfo1@@SG?AU_GUID@@XZ ; ICvPlayerOptionInfo1::GetInterfaceId
; Function compile flags: /Ogtpy
;	COMDAT ?GetInterfaceId@ICvPlayerOptionInfo1@@SG?AU_GUID@@XZ
_TEXT	SEGMENT
$T219030 = 8						; size = 4
?GetInterfaceId@ICvPlayerOptionInfo1@@SG?AU_GUID@@XZ PROC ; ICvPlayerOptionInfo1::GetInterfaceId, COMDAT

; 550  : 	static GUID DLLCALL GetInterfaceId() { return guidICvPlayerOptionInfo1; }

	mov	eax, DWORD PTR $T219030[esp-4]
	mov	ecx, DWORD PTR _guidICvPlayerOptionInfo1
	mov	edx, DWORD PTR _guidICvPlayerOptionInfo1+4
	mov	DWORD PTR [eax], ecx
	mov	ecx, DWORD PTR _guidICvPlayerOptionInfo1+8
	mov	DWORD PTR [eax+4], edx
	mov	edx, DWORD PTR _guidICvPlayerOptionInfo1+12
	mov	DWORD PTR [eax+8], ecx
	mov	DWORD PTR [eax+12], edx
	ret	4
?GetInterfaceId@ICvPlayerOptionInfo1@@SG?AU_GUID@@XZ ENDP ; ICvPlayerOptionInfo1::GetInterfaceId
_TEXT	ENDS
PUBLIC	??_7ICvPlayerOptionInfo1@@6B@			; ICvPlayerOptionInfo1::`vftable'
PUBLIC	??0ICvPlayerOptionInfo1@@QAE@XZ			; ICvPlayerOptionInfo1::ICvPlayerOptionInfo1
PUBLIC	??_R4ICvPlayerOptionInfo1@@6B@			; ICvPlayerOptionInfo1::`RTTI Complete Object Locator'
;	COMDAT ??_R4ICvPlayerOptionInfo1@@6B@
rdata$r	SEGMENT
??_R4ICvPlayerOptionInfo1@@6B@ DD 00H			; ICvPlayerOptionInfo1::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	FLAT:??_R0?AVICvPlayerOptionInfo1@@@8
	DD	FLAT:??_R3ICvPlayerOptionInfo1@@8
rdata$r	ENDS
;	COMDAT ??_7ICvPlayerOptionInfo1@@6B@
CONST	SEGMENT
??_7ICvPlayerOptionInfo1@@6B@ DD FLAT:??_R4ICvPlayerOptionInfo1@@6B@ ; ICvPlayerOptionInfo1::`vftable'
	DD	FLAT:__purecall
	DD	FLAT:__purecall
	DD	FLAT:__purecall
; Function compile flags: /Ogtpy
CONST	ENDS
;	COMDAT ??0ICvPlayerOptionInfo1@@QAE@XZ
_TEXT	SEGMENT
??0ICvPlayerOptionInfo1@@QAE@XZ PROC			; ICvPlayerOptionInfo1::ICvPlayerOptionInfo1, COMDAT
; _this$ = ecx
	mov	eax, ecx
	mov	DWORD PTR [eax], OFFSET ??_7ICvPlayerOptionInfo1@@6B@
	ret	0
??0ICvPlayerOptionInfo1@@QAE@XZ ENDP			; ICvPlayerOptionInfo1::ICvPlayerOptionInfo1
; Function compile flags: /Ogtpy
; File c:\users\enormousapplepie\documents\github\lekmod\lekmod_dll\cvgamecoredll_expansion2\cvdllplayeroptioninfo.cpp
_TEXT	ENDS
;	COMDAT ?QueryInterface@CvDllPlayerOptionInfo@@UAGPAXU_GUID@@@Z
_TEXT	SEGMENT
$T219042 = -16						; size = 16
$T219040 = -16						; size = 16
_this$ = 8						; size = 4
_guidInterface$ = 12					; size = 16
?QueryInterface@CvDllPlayerOptionInfo@@UAGPAXU_GUID@@@Z PROC ; CvDllPlayerOptionInfo::QueryInterface, COMDAT

; 28   : {

	sub	esp, 16					; 00000010H

; 29   : 	if(guidInterface == ICvUnknown::GetInterfaceId() ||
; 30   : 	        guidInterface == ICvPlayerOptionInfo1::GetInterfaceId())

	mov	ecx, DWORD PTR _guidICvUnknown+4
	mov	edx, DWORD PTR _guidICvUnknown+8
	mov	eax, DWORD PTR _guidICvUnknown
	mov	DWORD PTR $T219040[esp+20], ecx
	lea	ecx, DWORD PTR $T219040[esp+16]
	mov	DWORD PTR $T219040[esp+24], edx
	mov	DWORD PTR $T219040[esp+16], eax
	mov	eax, DWORD PTR _guidICvUnknown+12
	push	ecx
	lea	edx, DWORD PTR _guidInterface$[esp+16]
	push	edx
	mov	DWORD PTR $T219040[esp+36], eax
	call	_IsEqualGUID
	add	esp, 8
	test	eax, eax
	jne	SHORT $LN1@QueryInter
	mov	ecx, DWORD PTR _guidICvPlayerOptionInfo1+4
	mov	edx, DWORD PTR _guidICvPlayerOptionInfo1+8
	mov	eax, DWORD PTR _guidICvPlayerOptionInfo1
	mov	DWORD PTR $T219042[esp+20], ecx
	lea	ecx, DWORD PTR $T219042[esp+16]
	mov	DWORD PTR $T219042[esp+24], edx
	mov	DWORD PTR $T219042[esp+16], eax
	mov	eax, DWORD PTR _guidICvPlayerOptionInfo1+12
	push	ecx
	lea	edx, DWORD PTR _guidInterface$[esp+16]
	push	edx
	mov	DWORD PTR $T219042[esp+36], eax
	call	_IsEqualGUID
	add	esp, 8
	test	eax, eax
	jne	SHORT $LN1@QueryInter

; 33   : 		return this;
; 34   : 	}
; 35   : 
; 36   : 	return NULL;
; 37   : }

	add	esp, 16					; 00000010H
	ret	20					; 00000014H
$LN1@QueryInter:

; 31   : 	{
; 32   : 		IncrementReference();

	mov	eax, DWORD PTR _this$[esp+12]
	inc	DWORD PTR [eax+4]

; 33   : 		return this;
; 34   : 	}
; 35   : 
; 36   : 	return NULL;
; 37   : }

	add	esp, 16					; 00000010H
	ret	20					; 00000014H
?QueryInterface@CvDllPlayerOptionInfo@@UAGPAXU_GUID@@@Z ENDP ; CvDllPlayerOptionInfo::QueryInterface
_TEXT	ENDS
PUBLIC	??_GCvDllPlayerOptionInfo@@QAEPAXI@Z		; CvDllPlayerOptionInfo::`scalar deleting destructor'
; Function compile flags: /Ogtpy
;	COMDAT ??_GCvDllPlayerOptionInfo@@QAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8						; size = 4
??_GCvDllPlayerOptionInfo@@QAEPAXI@Z PROC		; CvDllPlayerOptionInfo::`scalar deleting destructor', COMDAT
; _this$ = ecx
	test	BYTE PTR ___flags$[esp-4], 1
	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET ??_7CvDllPlayerOptionInfo@@6B@
	je	SHORT $LN8@scalar
	push	esi
	call	?Free@CvDllGameContext@@SAXPAX@Z	; CvDllGameContext::Free
	add	esp, 4
$LN8@scalar:
	mov	eax, esi
	pop	esi
	ret	4
??_GCvDllPlayerOptionInfo@@QAEPAXI@Z ENDP		; CvDllPlayerOptionInfo::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0CvDllPlayerOptionInfo@@QAE@PAVCvPlayerOptionInfo@@@Z ; CvDllPlayerOptionInfo::CvDllPlayerOptionInfo
; Function compile flags: /Ogtpy
;	COMDAT ??0CvDllPlayerOptionInfo@@QAE@PAVCvPlayerOptionInfo@@@Z
_TEXT	SEGMENT
_pPlayerOptionInfo$ = 8					; size = 4
??0CvDllPlayerOptionInfo@@QAE@PAVCvPlayerOptionInfo@@@Z PROC ; CvDllPlayerOptionInfo::CvDllPlayerOptionInfo, COMDAT
; _this$ = ecx

; 19   : {

	mov	eax, ecx
	mov	ecx, DWORD PTR _pPlayerOptionInfo$[esp-4]
	mov	DWORD PTR [eax], OFFSET ??_7CvDllPlayerOptionInfo@@6B@
	mov	DWORD PTR [eax+4], 1
	mov	DWORD PTR [eax+8], ecx

; 20   : 	FAssertMsg(pPlayerOptionInfo != NULL, "SHOULD NOT HAPPEN");
; 21   : }

	ret	4
??0CvDllPlayerOptionInfo@@QAE@PAVCvPlayerOptionInfo@@@Z ENDP ; CvDllPlayerOptionInfo::CvDllPlayerOptionInfo
_TEXT	ENDS
PUBLIC	?DecrementReference@CvDllPlayerOptionInfo@@QAEIXZ ; CvDllPlayerOptionInfo::DecrementReference
; Function compile flags: /Ogtpy
;	COMDAT ?DecrementReference@CvDllPlayerOptionInfo@@QAEIXZ
_TEXT	SEGMENT
?DecrementReference@CvDllPlayerOptionInfo@@QAEIXZ PROC	; CvDllPlayerOptionInfo::DecrementReference, COMDAT
; _this$ = ecx

; 47   : 	if(m_uiRefCount == 1)

	mov	eax, DWORD PTR [ecx+4]
	cmp	eax, 1
	jne	SHORT $LN2@DecrementR

; 48   : 	{
; 49   : 		delete this;

	push	ecx
	mov	DWORD PTR [ecx], OFFSET ??_7CvDllPlayerOptionInfo@@6B@
	call	?Free@CvDllGameContext@@SAXPAX@Z	; CvDllGameContext::Free
	add	esp, 4

; 50   : 		return 0;

	xor	eax, eax

; 55   : 		return m_uiRefCount;
; 56   : 	}
; 57   : }

	ret	0
$LN2@DecrementR:

; 51   : 	}
; 52   : 	else
; 53   : 	{
; 54   : 		--m_uiRefCount;

	dec	eax
	mov	DWORD PTR [ecx+4], eax

; 55   : 		return m_uiRefCount;
; 56   : 	}
; 57   : }

	ret	0
?DecrementReference@CvDllPlayerOptionInfo@@QAEIXZ ENDP	; CvDllPlayerOptionInfo::DecrementReference
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?Destroy@CvDllPlayerOptionInfo@@EAGXXZ
_TEXT	SEGMENT
_this$ = 8						; size = 4
?Destroy@CvDllPlayerOptionInfo@@EAGXXZ PROC		; CvDllPlayerOptionInfo::Destroy, COMDAT

; 66   : 	DecrementReference();

	mov	eax, DWORD PTR _this$[esp-4]
	mov	ecx, DWORD PTR [eax+4]
	cmp	ecx, 1
	jne	SHORT $LN4@Destroy
	push	eax
	mov	DWORD PTR [eax], OFFSET ??_7CvDllPlayerOptionInfo@@6B@
	call	?Free@CvDllGameContext@@SAXPAX@Z	; CvDllGameContext::Free
	add	esp, 4

; 67   : }

	ret	4

; 66   : 	DecrementReference();

$LN4@Destroy:
	dec	ecx
	mov	DWORD PTR [eax+4], ecx

; 67   : }

	ret	4
?Destroy@CvDllPlayerOptionInfo@@EAGXXZ ENDP		; CvDllPlayerOptionInfo::Destroy
_TEXT	ENDS
END