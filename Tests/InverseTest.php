<?php

/**
 * Copyright © 2019. All rights reserved.
 *
 * @author Igor Agapie <igoragapie@gmail.com>
 */

declare(strict_types=1);

namespace Nump\Tests;

use Nump\Matrix;
use PHPUnit\Framework\TestCase;

class InverseTest extends TestCase
{
    /**
     * @return iterable
     */
    public function provider(): iterable
    {
        yield [
            [[5., 3., 1.], [3., 9., 4.], [1., 3., 5.]],
            [[0.25, -0.091, 0.023], [-0.083, 0.182, -0.129], [0, -0.091, 0.273]],
        ];

        yield [
            [
                [0.05041302, 0.22125276, 0.60900819, 0.7827549],
                [0.72885331, 0.00965047, 0.03653146, 0.42360486],
                [0.08119958, 0.61019517, 0.84375958, 0.04340501],
                [0.326979, 0.88669279, 0.76185049, 0.46199796],
            ],
            [
                [-0.64314727, 1.49216954, 0.71141341, -0.34533161],
                [-0.86386965, -0.83952152, -1.48247183, 2.37267348],
                [0.63149516, 0.47529014, 2.26006898, -1.7180584],
                [1.07181803, -0.22859507, -1.38518972, 0.68828735],
            ],
        ];
    }

    /**
     * @dataProvider provider
     *
     * @param array $matrix
     * @param array $expected
     */
    public function testMatrix(array $matrix, array $expected): void
    {
        $m = new Matrix($matrix);

        $inverse = $m->inverse();

        $actual = $this->round($inverse->toArray());
        $expected = $this->round($expected);

        $this->assertEquals($expected, $actual);
    }

    /**
     * @param array $data
     * @return array
     */
    private function round(array $data): array
    {
        return array_map(function ($item) {
            return is_array($item) ? $this->round($item) : round($item, 3);
        }, $data);
    }
}
