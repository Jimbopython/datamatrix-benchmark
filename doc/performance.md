# Performance

The following data shows the performance on an i5 10300H on Windows 11, when the amount of
datamatrix codes in an image is known.

| Library                        | Timeout | Mean   | Std. Dev. |
|--------------------------------|---------|--------|-----------|
| ZXingCodeReader                | 0ms     | 1.92ms | 2.57ms    |
| LibdmtxCodeReader              | 0ms     | 504ms  | 1170ms    |
| LibdmtxCodeReader              | 100ms   | 42ms   | 50ms      |
| LibdmtxCodeReader              | 200ms   | 75ms   | 106 ms    |
| LibdmtxZXingCombinedCodeReader | 0ms     | 508ms  | 1170ms    |
| LibdmtxZXingCombinedCodeReader | 100ms   | 43ms   | 51ms      |
| LibdmtxZXingCombinedCodeReader | 200ms   | 75ms   | 106ms     |