#!/bin/bash

TOTAL=107374182

USED=$(du -sb | cut -f1)

FREE=$((TOTAL - USED))

PERCENT=$((USED * 100 / TOTAL))

echo "Panaudota vieta (baitais): $USED"
echo "Liko laisvos vietos (baitais): $FREE"
echo "Panaudota vietos (procentais): $PERCENT"
echo "Daugiausiai vietos užimantys failai ir katalogai:"
du -sb * | sort -rh
