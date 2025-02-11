# Quadrature Encoder

This Arduino library is designed as a proof of concept for an ultra-fast bitwise-based read for a quadrature encoder.

## Algorithm

Take `~B1` (inverse bit 1 of current encoder value) and shift it right into `A1B1`. If this new `ATBT` (ABTest) value matches `A0B0` then the encoder has turned left, since this new value is left of the old value by one. Otherwise, it has turned right.  
  
Logic behind this is that on values where B1 is zero, the correct bit to shift into it is inverse B1.  

#### Step by Step

Old values

| A0 | B0 |
|-- | -- |
|0 |0 |

New Values:

| A1 | B1 |
|-- | -- |
|0 |1 |

Take `B1` and inverse it (~), equaling `0`.

Shift this value, `~B1 = 0`, right into the `A1B1` value to make:

Test Value:

| AT | BT |
|-- | -- |
|0 |0 |

This operation can be completed with:

```java
ATBT = A1B1 >> 1 | (~B1 << 1);  // Shift A1B1 right, then set the A1 digit to the inverse of B1
```

This new test value can be tested for equivalency with the old value via:

```java
~(A0B0 ^ ATBT)
```

Where a `1` (equivalency) will mean a left turn, and a `0` will mean a right turn. Real world use cases should also implement an initial test to see if the original values are equivalent and return a `NULL` value to indicate no rotation.

Alternatively, using an interrupt to trigger an ISR on a pin for any state change would avoid polling or branching to check for a change.

The encoder's position value can be modified without branching using:

```cpp
encoderValue += ((int(AOldBOld ^ ATestBTest) * 2) - 1);
````
