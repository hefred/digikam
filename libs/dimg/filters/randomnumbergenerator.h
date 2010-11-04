/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2010-11-03
 * Description : Generating random numbers
 *
 * Copyright (C) 2010 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

// Qt includes

#include <QByteArray>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT NonDeterministicRandomData : public QByteArray
{
public:

    /**
     * Constructs a QByteArray of given byte size
     * filled with non-deterministic random data.
     * For larger quantities of data,
     * prefer using a RandomNumberGenerator seeded
     * with non-deterministic data.
     */
    NonDeterministicRandomData(int size);
};

// --------------------------------------------------------------------

class DIGIKAM_EXPORT RandomNumberGenerator
{
public:

    /**
     * This class differs from standard pseudo
     * random number generators (rand()) in these points:
     * - it uses a specified, independently implemented algorithm
     *   identical across platforms
     * - provides access to the used seed
     * - it can thus guarantee replayable sequences
     * - it provides convenient seeding of varying quality
     */

    /**
     * Constructs a random number generator that is seeded
     * with a constant value. It is recommended to call a seed method
     * after construction.
     */
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    /**
     * Seeds the generator from a non-deterministic
     * random number generator. This is the most secure
     * seeding method.
     */
    void seedNonDeterministic();

    /**
     * Seeds the generator by current time. This is common practice
     * and good enough for most purposes.
     */
    void seedByTime();

    /**
     * Seeds the generator with the given value.
     * This is not meant to be called with a constant value,
     * but with a value retrieved from currentSeed() on a previous run.
     * Across platforms, the same sequence of random numbers will be
     * generated for the same seed.
     */
    void seed(quint32 seed);

    /**
     * Retrieves the current seed. Can be used for seed(quint32)
     * to replay the results again.
     */
    quint32 currentSeed() const;

    /**
     * Returns a random integer in the interval [min, max]
     * (including min and max)
     */
    int number(int min, int max);

    /**
     * Returns a random double in the interval [min, max)
     * (including min, excluding max)
     */
    double number(double min, double max);

    /**
     * Returns true with a probability of p
     * (where p shall be in the interval [0, 1])
     */
    bool yesOrNo(double p);

private:

    class RandomNumberGeneratorPriv;
    RandomNumberGeneratorPriv* const d;
};

} // namespace Digikam

#endif // RANDOMNUMBERGENERATOR_H
