C*GREXEC -- PGPLOT device handler dispatch routine
C 12/93 C. T. Dum: version for MS F32 Power Station
C+
      SUBROUTINE GREXEC(IDEV,IFUNC,RBUF,NBUF,CHR,LCHR)
      INTEGER IDEV, IFUNC, NBUF, LCHR
      REAL    RBUF(*)
      CHARACTER*(*) CHR
C---
      INTEGER NDEV
      PARAMETER (NDEV=7)
      CHARACTER*10 MSG
C---
      GOTO(1,2,3,4,5,6,7) IDEV
      IF (IDEV.EQ.0) THEN
          RBUF(1) = NDEV
          NBUF = 1
      ELSE
          WRITE (MSG,'(I10)') IDEV
          CALL GRQUIT('Unknown device code in GREXEC: '//MSG)
      END IF
      RETURN
C---
    1 CALL NUDRIV(IFUNC,RBUF,NBUF,CHR,LCHR)
      RETURN
    2 CALL MSDRIV(IFUNC,RBUF,NBUF,CHR,LCHR)
      RETURN
    3 CALL PSDRIV(IFUNC,RBUF,NBUF,CHR,LCHR,1)
      RETURN
    4 CALL PSDRIV(IFUNC,RBUF,NBUF,CHR,LCHR,2)
      RETURN
    5 CALL PSDRIV(IFUNC,RBUF,NBUF,CHR,LCHR,3)
      RETURN
    6 CALL PSDRIV(IFUNC,RBUF,NBUF,CHR,LCHR,4)
      RETURN
    7 CALL LXDRIV(IFUNC,RBUF,NBUF,CHR,LCHR)
      RETURN
C    8 CALL HJDRIV(IFUNC,RBUF,NBUF,CHR,LCHR)
C      RETURN
      END
