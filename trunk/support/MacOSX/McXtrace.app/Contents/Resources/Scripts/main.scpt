FasdUAS 1.101.10   ��   ��    k             l     ��  ��    9 3 Generic Script Launcher with Support for Arguments     � 	 	 f   G e n e r i c   S c r i p t   L a u n c h e r   w i t h   S u p p o r t   f o r   A r g u m e n t s   
  
 l     ��  ��    4 . by Martin Fuhrer (mfuhrer@alumni.ucalgary.ca)     �   \   b y   M a r t i n   F u h r e r   ( m f u h r e r @ a l u m n i . u c a l g a r y . c a )      l     ��  ��    3 - Permission to modify and distribute granted.     �   Z   P e r m i s s i o n   t o   m o d i f y   a n d   d i s t r i b u t e   g r a n t e d .      l     ��������  ��  ��        l     ��  ��    \ V This Applescript droplet calls the script myScript inside the application bundle, and     �   �   T h i s   A p p l e s c r i p t   d r o p l e t   c a l l s   t h e   s c r i p t   m y S c r i p t   i n s i d e   t h e   a p p l i c a t i o n   b u n d l e ,   a n d      l     ��  ��    F @ passes any file arguments from the droplet along to the script.     �   �   p a s s e s   a n y   f i l e   a r g u m e n t s   f r o m   t h e   d r o p l e t   a l o n g   t o   t h e   s c r i p t .     !   l     ��������  ��  ��   !  " # " l     ��������  ��  ��   #  $ % $ l      & ' ( & j     �� )��  0 scriptlocation scriptLocation ) m      * * � + + & C o n t e n t s : R e s o u r c e s : ' 3 - location of script within application bundle    ( � , , Z   l o c a t i o n   o f   s c r i p t   w i t h i n   a p p l i c a t i o n   b u n d l e %  - . - l      / 0 1 / j    �� 2�� 0 myscript myScript 2 m     3 3 � 4 4  l a u n c h e r . s h 0   name of script    1 � 5 5    n a m e   o f   s c r i p t .  6 7 6 l     ��������  ��  ��   7  8 9 8 l     �� : ;��   : D > code gets called when application is double-clicked or opened    ; � < < |   c o d e   g e t s   c a l l e d   w h e n   a p p l i c a t i o n   i s   d o u b l e - c l i c k e d   o r   o p e n e d 9  = > = l    	 ?���� ? r     	 @ A @ I    �� B C
�� .earsffdralis        afdr B  f      C �� D��
�� 
rtyp D m    ��
�� 
TEXT��   A o      ���� 0 apppath appPath��  ��   >  E F E l  
  G���� G r   
  H I H b   
  J K J b   
  L M L o   
 ���� 0 apppath appPath M o    ����  0 scriptlocation scriptLocation K o    ���� 0 myscript myScript I o      ���� 0 
scriptpath 
scriptPath��  ��   F  N O N l     ��������  ��  ��   O  P Q P l   # R���� R r    # S T S n    ! U V U 1    !��
�� 
strq V l    W���� W c     X Y X n     Z [ Z 1    ��
�� 
psxp [ o    ���� 0 
scriptpath 
scriptPath Y m    ��
�� 
TEXT��  ��   T o      ���� 0 	posixpath 	posixPath��  ��   Q  \ ] \ l  $ ' ^���� ^ r   $ ' _ ` _ o   $ %���� 0 	posixpath 	posixPath ` o      ���� 0 command  ��  ��   ]  a b a l     ��������  ��  ��   b  c d c l  ( A e���� e Q   ( A f g h f r   + 2 i j i I  + 0�� k��
�� .sysoexecTEXT���     TEXT k o   + ,���� 0 command  ��   j o      ���� 0 results   g R      �� l m
�� .ascrerr ****      � **** l o      ���� 0 	errortext 	errorText m �� n��
�� 
errn n o      ���� 0 errornumber errorNumber��   h I   : A�� o���� 0 errormessage errorMessage o  p q p o   ; <���� 0 errornumber errorNumber q  r�� r o   < =���� 0 	errortext 	errorText��  ��  ��  ��   d  s t s l     ��������  ��  ��   t  u v u l     �� w x��   w I C subroutine gets called when files are dropped onto the application    x � y y �   s u b r o u t i n e   g e t s   c a l l e d   w h e n   f i l e s   a r e   d r o p p e d   o n t o   t h e   a p p l i c a t i o n v  z { z i    	 | } | I     �� ~��
�� .aevtodocnull  �    alis ~ o      ���� 0 argnames argNames��   } k     p    � � � r      � � � m      � � � � �   � o      ���� 0 arglist argList �  � � � X    * ��� � � k    % � �  � � � r     � � � n     � � � 1    ��
�� 
strq � l    ����� � c     � � � n     � � � 1    ��
�� 
psxp � o    ���� 0 curpath curPath � m    ��
�� 
TEXT��  ��   � o      ���� 0 	posixpath 	posixPath �  ��� � r    % � � � b    # � � � b    ! � � � o    ���� 0 arglist argList � o     ���� 0 	posixpath 	posixPath � m   ! " � � � � �    � o      ���� 0 arglist argList��  �� 0 curpath curPath � o    ���� 0 argnames argNames �  � � � l  + +��������  ��  ��   �  � � � r   + 4 � � � I  + 2�� � �
�� .earsffdralis        afdr �  f   + , � �� ���
�� 
rtyp � m   - .��
�� 
TEXT��   � o      ���� 0 apppath appPath �  � � � r   5 D � � � b   5 B � � � b   5 < � � � o   5 6���� 0 apppath appPath � o   6 ;����  0 scriptlocation scriptLocation � o   < A���� 0 myscript myScript � o      ���� 0 
scriptpath 
scriptPath �  � � � l  E E��������  ��  ��   �  � � � r   E N � � � n   E L � � � 1   J L��
�� 
strq � l  E J ����� � c   E J � � � n   E H � � � 1   F H��
�� 
psxp � o   E F���� 0 
scriptpath 
scriptPath � m   H I��
�� 
TEXT��  ��   � o      ���� 0 	posixpath 	posixPath �  � � � r   O V � � � b   O T � � � b   O R � � � o   O P���� 0 	posixpath 	posixPath � m   P Q � � � � �    � o   R S���� 0 arglist argList � o      ���� 0 command   �  � � � l  W W��������  ��  ��   �  ��� � Q   W p � � � � r   Z a � � � I  Z _�� ���
�� .sysoexecTEXT���     TEXT � o   Z [���� 0 command  ��   � o      ���� 0 results   � R      �� � �
�� .ascrerr ****      � **** � o      ���� 0 	errortext 	errorText � �� ���
�� 
errn � o      ���� 0 errornumber errorNumber��   � I   i p� ��~� 0 errormessage errorMessage �  � � � o   j k�}�} 0 errornumber errorNumber �  ��| � o   k l�{�{ 0 	errortext 	errorText�|  �~  ��   {  � � � l     �z�y�x�z  �y  �x   �  ��w � i   
  � � � I      �v ��u�v 0 errormessage errorMessage �  � � � o      �t�t 0 errornumber errorNumber �  ��s � o      �r�r 0 	errortext 	errorText�s  �u   � k     k � �  � � � Z     & � ��q�p � =     � � � o     �o�o 0 errornumber errorNumber � m    �n�n  � k    " � �  � � � r     � � � n    
 � � � 4   
�m �
�m 
cwor � m    	�l�l  � o    �k�k 0 	errortext 	errorText � o      �j�j 0 programname programName �  ��i � I   "�h � �
�h .sysodlogaskr        TEXT � b     � � � b     � � � b     � � � b     � � � b     � � � m     � � � � �  X D r o p l e t s   E r r o r � o    �g
�g 
ret  � o    �f
�f 
ret  � l 	   ��e�d � m     � � �   < U n a b l e   t o   l a u n c h   t h e   p r o g r a m   '�e  �d   � o    �c�c 0 programname programName � m     � � ' ,   a s   i t   d o e s   n o t   a p p e a r   i n   y o u r   p a t h .     P l e a s e   m a k e   s u r e   t h e   p r o g r a m   i s   p r o p e r l y   i n s t a l l e d . � �b
�b 
btns J     �a m     �  O K�a   �`	�_
�` 
dflt	 m    �^�^ �_  �i  �q  �p   � 

 l  ' '�]�\�[�]  �\  �[    Z   ' M�Z�Y =  ' * o   ' (�X�X 0 errornumber errorNumber m   ( )�W�W  k   - I  r   - 3 n   - 1 4  . 1�V
�V 
cwor m   / 0�U�U�� o   - .�T�T 0 	errortext 	errorText o      �S�S  0 x11environment x11Environment �R I  4 I�Q
�Q .sysodlogaskr        TEXT b   4 ? b   4 =  b   4 ;!"! b   4 9#$# b   4 7%&% m   4 5'' �((  X D r o p l e t s   E r r o r& o   5 6�P
�P 
ret $ o   7 8�O
�O 
ret " l 	 9 :)�N�M) m   9 :** �++ > Y o u r   s e l e c t e d   X 1 1   e n v i r o n m e n t   '�N  �M    o   ; <�L�L  0 x11environment x11Environment m   = >,, �-- � . a p p '   c o u l d   n o t   b e   l a u n c h e d .   P l e a s e   u s e   t h e   X D r o p l e t s   S e t u p   p r o g r a m   t o   s e l e c t   y o u r   X 1 1   e n v i r o n m e n t . �K./
�K 
btns. J   @ C00 1�J1 m   @ A22 �33  O K�J  / �I4�H
�I 
dflt4 m   D E�G�G �H  �R  �Z  �Y   565 l  N N�F�E�D�F  �E  �D  6 7�C7 Z   N k89�B�A8 =  N Q:;: o   N O�@�@ 0 errornumber errorNumber; m   O P�?�? 9 I  T g�><=
�> .sysodlogaskr        TEXT< b   T [>?> b   T Y@A@ b   T WBCB m   T UDD �EE  X D r o p l e t s   E r r o rC o   U V�=
�= 
ret A o   W X�<
�< 
ret ? l 	 Y ZF�;�:F m   Y ZGG �HH � U n a b l e   t o   o b t a i n   d i s p l a y   v a r i a b l e   f r o m   / t m p / . X 1 1 - u n i x .     P l e a s e   m a k e   s u r e   y o u r   X 1 1   e n v i r o n m e n t   i s   s t a r t i n g   p r o p e r l y .�;  �:  = �9IJ
�9 
btnsI J   \ aKK L�8L m   \ _MM �NN  O K�8  J �7O�6
�7 
dfltO m   b c�5�5 �6  �B  �A  �C  �w       �4P * 3QRSTUVVW�4  P 
�3�2�1�0�/�.�-�,�+�*�3  0 scriptlocation scriptLocation�2 0 myscript myScript
�1 .aevtodocnull  �    alis�0 0 errormessage errorMessage
�/ .aevtoappnull  �   � ****�. 0 apppath appPath�- 0 
scriptpath 
scriptPath�, 0 	posixpath 	posixPath�+ 0 command  �* 0 results  Q �) }�(�'XY�&
�) .aevtodocnull  �    alis�( 0 argnames argNames�'  X 
�%�$�#�"�!� �����% 0 argnames argNames�$ 0 arglist argList�# 0 curpath curPath�" 0 	posixpath 	posixPath�! 0 apppath appPath�  0 
scriptpath 
scriptPath� 0 command  � 0 results  � 0 	errortext 	errorText� 0 errornumber errorNumberY  ������� ��� ���Z�
� 
kocl
� 
cobj
� .corecnte****       ****
� 
psxp
� 
TEXT
� 
strq
� 
rtyp
� .earsffdralis        afdr
� .sysoexecTEXT���     TEXT� 0 	errortext 	errorTextZ ���
� 
errn� 0 errornumber errorNumber�  � 0 errormessage errorMessage�& q�E�O %�[��l kh ��,�&�,E�O��%�%E�[OY��O)��l 	E�O�b   %b  %E�O��,�&�,E�O��%�%E�O �j E�W X  *��l+ R � ���[\�
� 0 errormessage errorMessage� �	]�	 ]  ��� 0 errornumber errorNumber� 0 	errortext 	errorText�  [ ����� 0 errornumber errorNumber� 0 	errortext 	errorText� 0 programname programName�  0 x11environment x11Environment\ � �� �� ������'*,2DGM
� 
cwor
� 
ret 
�  
btns
�� 
dflt�� 
�� .sysodlogaskr        TEXT�
 l�k  !��k/E�O��%�%�%�%�%��kv�k� 	Y hO�l  !��i/E�O��%�%�%�%�%��kv�k� 	Y hO�m  ��%�%�%�a kv�k� 	Y hS ��^����_`��
�� .aevtoappnull  �   � ****^ k     Aaa  =bb  Ecc  Pdd  \ee  c����  ��  ��  _ ������ 0 	errortext 	errorText�� 0 errornumber errorNumber` ������������������������f��
�� 
rtyp
�� 
TEXT
�� .earsffdralis        afdr�� 0 apppath appPath�� 0 
scriptpath 
scriptPath
�� 
psxp
�� 
strq�� 0 	posixpath 	posixPath�� 0 command  
�� .sysoexecTEXT���     TEXT�� 0 results  �� 0 	errortext 	errorTextf ������
�� 
errn�� 0 errornumber errorNumber��  �� 0 errormessage errorMessage�� B)��l E�O�b   %b  %E�O��,�&�,E�O�E�O �j 	E�W X  *��l+ T �gg � M a c i n t o s h   H D : U s e r s : w i l l e n d : P r o j e c t s : M c C o d e : t r u n k : d i s t : M c X t r a c e . a p p :U �hh � M a c i n t o s h   H D : U s e r s : w i l l e n d : P r o j e c t s : M c C o d e : t r u n k : d i s t : M c X t r a c e . a p p : C o n t e n t s : R e s o u r c e s : l a u n c h e r . s hV �ii � ' / U s e r s / w i l l e n d / P r o j e c t s / M c C o d e / t r u n k / d i s t / M c X t r a c e . a p p / C o n t e n t s / R e s o u r c e s / l a u n c h e r . s h 'W �jj  ascr  ��ޭ