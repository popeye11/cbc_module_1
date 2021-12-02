/*******************************************************************************
 *
 * @copyright   TOPTICA Photonics AG
 * @date        2018 - 2020
 *
 * @file        test_data.cpp
 * @brief       Test data for TOPTICA TSP unit tests.
 *
 * @author      Fuchs, Daniel <Daniel.Fuchs@toptica.com>
 * @author      Hager, Manfred <Manfred.Hager@toptica.com>
 * @author      Hempel, Felix <Felix.Hempel@toptica.com>
 * @author      Lopes, Emilio <Emilio.Lopes@toptica.com>
 * @author      Rehme, Paul <Paul.Rehme@toptica.com>
 * @author      Roggenbuck, Axel <Axel.Roggenbuck@toptica.com>
 * @author      Zhang, Xiaodong <Xiaodong.Zhang@toptica.com>
 *
 ******************************************************************************/
#include <test_data.hpp>


namespace toptica::test::data {

std::array<float, 1000> butterworth_2_order_low_pass_1_500_impulse_response{{
    3.91302092e-05F,
    1.55825459e-04F,
    3.09570984e-04F,
    4.60559881e-04F,
    6.08817092e-04F,
    7.54367444e-04F,
    8.97235819e-04F,
    1.03744725e-03F,
    1.17502653e-03F,
    1.30999845e-03F,
    1.44238793e-03F,
    1.57221954e-03F,
    1.69951795e-03F,
    1.82430807e-03F,
    1.94661447e-03F,
    2.06646184e-03F,
    2.18387460e-03F,
    2.29887711e-03F,
    2.41149426e-03F,
    2.52175005e-03F,
    2.62966868e-03F,
    2.73527461e-03F,
    2.83859251e-03F,
    2.93964590e-03F,
    3.03845899e-03F,
    3.13505577e-03F,
    3.22946021e-03F,
    3.32169607e-03F,
    3.41178733e-03F,
    3.49975750e-03F,
    3.58563010e-03F,
    3.66942887e-03F,
    3.75117688e-03F,
    3.83089762e-03F,
    3.90861463e-03F,
    3.98435071e-03F,
    4.05812915e-03F,
    4.12997277e-03F,
    4.19990439e-03F,
    4.26794635e-03F,
    4.33412101e-03F,
    4.39845212e-03F,
    4.46096063e-03F,
    4.52166982e-03F,
    4.58060158e-03F,
    4.63777781e-03F,
    4.69322130e-03F,
    4.74695256e-03F,
    4.79899440e-03F,
    4.84936778e-03F,
    4.89809457e-03F,
    4.94519621e-03F,
    4.99069318e-03F,
    5.03460784e-03F,
    5.07695973e-03F,
    5.11777075e-03F,
    5.15706139e-03F,
    5.19485259e-03F,
    5.23116440e-03F,
    5.26601728e-03F,
    5.29943174e-03F,
    5.33142686e-03F,
    5.36202407e-03F,
    5.39124245e-03F,
    5.41910250e-03F,
    5.44562377e-03F,
    5.47082536e-03F,
    5.49472636e-03F,
    5.51734539e-03F,
    5.53870248e-03F,
    5.55881532e-03F,
    5.57770440e-03F,
    5.59538696e-03F,
    5.61188255e-03F,
    5.62720886e-03F,
    5.64138358e-03F,
    5.65442583e-03F,
    5.66635188e-03F,
    5.67718130e-03F,
    5.68693038e-03F,
    5.69561729e-03F,
    5.70326019e-03F,
    5.70987491e-03F,
    5.71548007e-03F,
    5.72009105e-03F,
    5.72372507e-03F,
    5.72639983e-03F,
    5.72813116e-03F,
    5.72893489e-03F,
    5.72882686e-03F,
    5.72782336e-03F,
    5.72594116e-03F,
    5.72319562e-03F,
    5.71960164e-03F,
    5.71517553e-03F,
    5.70993172e-03F,
    5.70388651e-03F,
    5.69705525e-03F,
    5.68945194e-03F,
    5.68109145e-03F,
    5.67198824e-03F,
    5.66215673e-03F,
    5.65161277e-03F,
    5.64036891e-03F,
    5.62843960e-03F,
    5.61583927e-03F,
    5.60258143e-03F,
    5.58867957e-03F,
    5.57414815e-03F,
    5.55900019e-03F,
    5.54324780e-03F,
    5.52690448e-03F,
    5.50998421e-03F,
    5.49249910e-03F,
    5.47446124e-03F,
    5.45588415e-03F,
    5.43678040e-03F,
    5.41716255e-03F,
    5.39704133e-03F,
    5.37643116e-03F,
    5.35534089e-03F,
    5.33378497e-03F,
    5.31177223e-03F,
    5.28931478e-03F,
    5.26642520e-03F,
    5.24311420e-03F,
    5.21939201e-03F,
    5.19527122e-03F,
    5.17076114e-03F,
    5.14587294e-03F,
    5.12061734e-03F,
    5.09500504e-03F,
    5.06904535e-03F,
    5.04274946e-03F,
    5.01612714e-03F,
    4.98918816e-03F,
    4.96194232e-03F,
    4.93439939e-03F,
    4.90656914e-03F,
    4.87846043e-03F,
    4.85008350e-03F,
    4.82144626e-03F,
    4.79255849e-03F,
    4.76342905e-03F,
    4.73406631e-03F,
    4.70447959e-03F,
    4.67467727e-03F,
    4.64466773e-03F,
    4.61445935e-03F,
    4.58406098e-03F,
    4.55348007e-03F,
    4.52272501e-03F,
    4.49180324e-03F,
    4.46072267e-03F,
    4.42949124e-03F,
    4.39811591e-03F,
    4.36660508e-03F,
    4.33496526e-03F,
    4.30320390e-03F,
    4.27132845e-03F,
    4.23934590e-03F,
    4.20726324e-03F,
    4.17508651e-03F,
    4.14282363e-03F,
    4.11048066e-03F,
    4.07806318e-03F,
    4.04557819e-03F,
    4.01303172e-03F,
    3.98042984e-03F,
    3.94777907e-03F,
    3.91508453e-03F,
    3.88235296e-03F,
    3.84959020e-03F,
    3.81680205e-03F,
    3.78399342e-03F,
    3.75117036e-03F,
    3.71833751e-03F,
    3.68550117e-03F,
    3.65266623e-03F,
    3.61983757e-03F,
    3.58702056e-03F,
    3.55421938e-03F,
    3.52143962e-03F,
    3.48868570e-03F,
    3.45596229e-03F,
    3.42327426e-03F,
    3.39062605e-03F,
    3.35802184e-03F,
    3.32546630e-03F,
    3.29296314e-03F,
    3.26051726e-03F,
    3.22813191e-03F,
    3.19581153e-03F,
    3.16355983e-03F,
    3.13138030e-03F,
    3.09927762e-03F,
    3.06725525e-03F,
    3.03531671e-03F,
    3.00346548e-03F,
    2.97170482e-03F,
    2.94003822e-03F,
    2.90846918e-03F,
    2.87700072e-03F,
    2.84563610e-03F,
    2.81437812e-03F,
    2.78323027e-03F,
    2.75219488e-03F,
    2.72127474e-03F,
    2.69047311e-03F,
    2.65979255e-03F,
    2.62923562e-03F,
    2.59880512e-03F,
    2.56850407e-03F,
    2.53833481e-03F,
    2.50829989e-03F,
    2.47840118e-03F,
    2.44864123e-03F,
    2.41902191e-03F,
    2.38954555e-03F,
    2.36021425e-03F,
    2.33102986e-03F,
    2.30199471e-03F,
    2.27311021e-03F,
    2.24437891e-03F,
    2.21580220e-03F,
    2.18738196e-03F,
    2.15911982e-03F,
    2.13101739e-03F,
    2.10307632e-03F,
    2.07529822e-03F,
    2.04768451e-03F,
    2.02023657e-03F,
    1.99295557e-03F,
    1.96584291e-03F,
    1.93890021e-03F,
    1.91212830e-03F,
    1.88552856e-03F,
    1.85910228e-03F,
    1.83285028e-03F,
    1.80677383e-03F,
    1.78087386e-03F,
    1.75515120e-03F,
    1.72960700e-03F,
    1.70424185e-03F,
    1.67905679e-03F,
    1.65405241e-03F,
    1.62922952e-03F,
    1.60458882e-03F,
    1.58013101e-03F,
    1.55585667e-03F,
    1.53176638e-03F,
    1.50786084e-03F,
    1.48414029e-03F,
    1.46060530e-03F,
    1.43725623e-03F,
    1.41409342e-03F,
    1.39111688e-03F,
    1.36832742e-03F,
    1.34572526e-03F,
    1.32331019e-03F,
    1.30108278e-03F,
    1.27904292e-03F,
    1.25719095e-03F,
    1.23552687e-03F,
    1.21405092e-03F,
    1.19276321e-03F,
    1.17166364e-03F,
    1.15075242e-03F,
    1.13002909e-03F,
    1.10949401e-03F,
    1.08914694e-03F,
    1.06898765e-03F,
    1.04901625e-03F,
    1.02923228e-03F,
    1.00963563e-03F,
    9.90226166e-04F,
    9.71003552e-04F,
    9.51967551e-04F,
    9.33117699e-04F,
    9.14453762e-04F,
    8.95975274e-04F,
    8.77681829e-04F,
    8.59573309e-04F,
    8.41649075e-04F,
    8.23908718e-04F,
    8.06351891e-04F,
    7.88978010e-04F,
    7.71786552e-04F,
    7.54777051e-04F,
    7.37948867e-04F,
    7.21301651e-04F,
    7.04834645e-04F,
    6.88547443e-04F,
    6.72439171e-04F,
    6.56509306e-04F,
    6.40757266e-04F,
    6.25182351e-04F,
    6.09783805e-04F,
    5.94561105e-04F,
    5.79513377e-04F,
    5.64640039e-04F,
    5.49940160e-04F,
    5.35413157e-04F,
    5.21058158e-04F,
    5.06874523e-04F,
    4.92861262e-04F,
    4.79017704e-04F,
    4.65342950e-04F,
    4.51836153e-04F,
    4.38496529e-04F,
    4.25323204e-04F,
    4.12315334e-04F,
    3.99472017e-04F,
    3.86792468e-04F,
    3.74275667e-04F,
    3.61920800e-04F,
    3.49726877e-04F,
    3.37692996e-04F,
    3.25818197e-04F,
    3.14101577e-04F,
    3.02542176e-04F,
    2.91139004e-04F,
    2.79891130e-04F,
    2.68797536e-04F,
    2.57857260e-04F,
    2.47069343e-04F,
    2.36432752e-04F,
    2.25946496e-04F,
    2.15609572e-04F,
    2.05420976e-04F,
    1.95379675e-04F,
    1.85484663e-04F,
    1.75734895e-04F,
    1.66129350e-04F,
    1.56666996e-04F,
    1.47346815e-04F,
    1.38167714e-04F,
    1.29128661e-04F,
    1.20228622e-04F,
    1.11466521e-04F,
    1.02841310e-04F,
    9.43519190e-05F,
    8.59972934e-05F,
    7.77763707e-05F,
    6.96880888e-05F,
    6.17313563e-05F,
    5.39051216e-05F,
    4.62083008e-05F,
    3.86398206e-05F,
    3.11986078e-05F,
    2.38835819e-05F,
    1.66936607e-05F,
    9.62777449e-06F,
    2.68484382e-06F,
    -4.13621001e-06F,
    -1.08364666e-05F,
    -1.74170036e-05F,
    -2.38788998e-05F,
    -3.02232329e-05F,
    -3.64510815e-05F,
    -4.25635226e-05F,
    -4.85616256e-05F,
    -5.44464747e-05F,
    -6.02191394e-05F,
    -6.58806894e-05F,
    -7.14322014e-05F,
    -7.68747341e-05F,
    -8.22093643e-05F,
    -8.74371472e-05F,
    -9.25591667e-05F,
    -9.75764706e-05F,
    -1.02490114e-04F,
    -1.07301174e-04F,
    -1.12010675e-04F,
    -1.16619696e-04F,
    -1.21129262e-04F,
    -1.25540435e-04F,
    -1.29854248e-04F,
    -1.34071757e-04F,
    -1.38193966e-04F,
    -1.42221921e-04F,
    -1.46156672e-04F,
    -1.49999221e-04F,
    -1.53750618e-04F,
    -1.57411851e-04F,
    -1.60983967e-04F,
    -1.64467958e-04F,
    -1.67864840e-04F,
    -1.71175590e-04F,
    -1.74401241e-04F,
    -1.77542781e-04F,
    -1.80601186e-04F,
    -1.83577446e-04F,
    -1.86472549e-04F,
    -1.89287442e-04F,
    -1.92023144e-04F,
    -1.94680557e-04F,
    -1.97260670e-04F,
    -1.99764443e-04F,
    -2.02192852e-04F,
    -2.04546799e-04F,
    -2.06827273e-04F,
    -2.09035177e-04F,
    -2.11171457e-04F,
    -2.13237043e-04F,
    -2.15232896e-04F,
    -2.17159846e-04F,
    -2.19018853e-04F,
    -2.20810820e-04F,
    -2.22536662e-04F,
    -2.24197240e-04F,
    -2.25793454e-04F,
    -2.27326207e-04F,
    -2.28796343e-04F,
    -2.30204751e-04F,
    -2.31552331e-04F,
    -2.32839884e-04F,
    -2.34068284e-04F,
    -2.35238389e-04F,
    -2.36351014e-04F,
    -2.37407046e-04F,
    -2.38407287e-04F,
    -2.39352579e-04F,
    -2.40243724e-04F,
    -2.41081550e-04F,
    -2.41866874e-04F,
    -2.42600479e-04F,
    -2.43283168e-04F,
    -2.43915740e-04F,
    -2.44499010e-04F,
    -2.45033676e-04F,
    -2.45520554e-04F,
    -2.45960458e-04F,
    -2.46354088e-04F,
    -2.46702228e-04F,
    -2.47005606e-04F,
    -2.47264921e-04F,
    -2.47480988e-04F,
    -2.47654476e-04F,
    -2.47786171e-04F,
    -2.47876742e-04F,
    -2.47926917e-04F,
    -2.47937365e-04F,
    -2.47908814e-04F,
    -2.47841963e-04F,
    -2.47737480e-04F,
    -2.47596065e-04F,
    -2.47418357e-04F,
    -2.47205055e-04F,
    -2.46956799e-04F,
    -2.46674230e-04F,
    -2.46358046e-04F,
    -2.46008858e-04F,
    -2.45627307e-04F,
    -2.45214003e-04F,
    -2.44769588e-04F,
    -2.44294672e-04F,
    -2.43789909e-04F,
    -2.43255839e-04F,
    -2.42693117e-04F,
    -2.42102295e-04F,
    -2.41483998e-04F,
    -2.40838766e-04F,
    -2.40167181e-04F,
    -2.39469853e-04F,
    -2.38747307e-04F,
    -2.38000081e-04F,
    -2.37228756e-04F,
    -2.36433843e-04F,
    -2.35615938e-04F,
    -2.34775536e-04F,
    -2.33913219e-04F,
    -2.33029466e-04F,
    -2.32124759e-04F,
    -2.31199636e-04F,
    -2.30254605e-04F,
    -2.29290163e-04F,
    -2.28306817e-04F,
    -2.27305034e-04F,
    -2.26285294e-04F,
    -2.25248121e-04F,
    -2.24193966e-04F,
    -2.23123294e-04F,
    -2.22036542e-04F,
    -2.20934162e-04F,
    -2.19816604e-04F,
    -2.18684305e-04F,
    -2.17537716e-04F,
    -2.16377259e-04F,
    -2.15203356e-04F,
    -2.14016414e-04F,
    -2.12816871e-04F,
    -2.11605118e-04F,
    -2.10381579e-04F,
    -2.09146659e-04F,
    -2.07900739e-04F,
    -2.06644210e-04F,
    -2.05377481e-04F,
    -2.04100870e-04F,
    -2.02814816e-04F,
    -2.01519637e-04F,
    -2.00215727e-04F,
    -1.98903406e-04F,
    -1.97583067e-04F,
    -1.96255045e-04F,
    -1.94919674e-04F,
    -1.93577333e-04F,
    -1.92228326e-04F,
    -1.90872990e-04F,
    -1.89511629e-04F,
    -1.88144579e-04F,
    -1.86772144e-04F,
    -1.85394645e-04F,
    -1.84012388e-04F,
    -1.82625678e-04F,
    -1.81234791e-04F,
    -1.79840048e-04F,
    -1.78441711e-04F,
    -1.77040070e-04F,
    -1.75635389e-04F,
    -1.74227986e-04F,
    -1.72818080e-04F,
    -1.71405947e-04F,
    -1.69991836e-04F,
    -1.68576007e-04F,
    -1.67158709e-04F,
    -1.65740188e-04F,
    -1.64320707e-04F,
    -1.62900469e-04F,
    -1.61479751e-04F,
    -1.60058757e-04F,
    -1.58637733e-04F,
    -1.57216884e-04F,
    -1.55796457e-04F,
    -1.54376641e-04F,
    -1.52957669e-04F,
    -1.51539731e-04F,
    -1.50123029e-04F,
    -1.48707783e-04F,
    -1.47294180e-04F,
    -1.45882426e-04F,
    -1.44472710e-04F,
    -1.43065205e-04F,
    -1.41660086e-04F,
    -1.40257529e-04F,
    -1.38857722e-04F,
    -1.37460825e-04F,
    -1.36067014e-04F,
    -1.34676462e-04F,
    -1.33289301e-04F,
    -1.31905719e-04F,
    -1.30525834e-04F,
    -1.29149819e-04F,
    -1.27777836e-04F,
    -1.26409999e-04F,
    -1.25046456e-04F,
    -1.23687350e-04F,
    -1.22332829e-04F,
    -1.20983008e-04F,
    -1.19638018e-04F,
    -1.18297976e-04F,
    -1.16963020e-04F,
    -1.15633258e-04F,
    -1.14308816e-04F,
    -1.12989801e-04F,
    -1.11676345e-04F,
    -1.10368535e-04F,
    -1.09066488e-04F,
    -1.07770291e-04F,
    -1.06480053e-04F,
    -1.05195882e-04F,
    -1.03917861e-04F,
    -1.02646067e-04F,
    -1.01380610e-04F,
    -1.00121564e-04F,
    -9.88690299e-05F,
    -9.76230731e-05F,
    -9.63837883e-05F,
    -9.51512411e-05F,
    -9.39254969e-05F,
    -9.27066503e-05F,
    -9.14947450e-05F,
    -9.02898537e-05F,
    -8.90920637e-05F,
    -8.79014115e-05F,
    -8.67179697e-05F,
    -8.55418039e-05F,
    -8.43729576e-05F,
    -8.32114893e-05F,
    -8.20574496e-05F,
    -8.09108897e-05F,
    -7.97718603e-05F,
    -7.86404053e-05F,
    -7.75165754e-05F,
    -7.64004071e-05F,
    -7.52919295e-05F,
    -7.41911936e-05F,
    -7.30982283e-05F,
    -7.20130774e-05F,
    -7.09357555e-05F,
    -6.98663061e-05F,
    -6.88047585e-05F,
    -6.77511198e-05F,
    -6.67054410e-05F,
    -6.56677366e-05F,
    -6.46380286e-05F,
    -6.36163386e-05F,
    -6.26026886e-05F,
    -6.15970930e-05F,
    -6.05995629e-05F,
    -5.96101127e-05F,
    -5.86287533e-05F,
    -5.76554994e-05F,
    -5.66903545e-05F,
    -5.57333369e-05F,
    -5.47844502e-05F,
    -5.38437052e-05F,
    -5.29110948e-05F,
    -5.19866298e-05F,
    -5.10703067e-05F,
    -5.01621253e-05F,
    -4.92620929e-05F,
    -4.83701988e-05F,
    -4.74864464e-05F,
    -4.66108249e-05F,
    -4.57433234e-05F,
    -4.48839419e-05F,
    -4.40326694e-05F,
    -4.31894950e-05F,
    -4.23544116e-05F,
    -4.15274044e-05F,
    -4.07084663e-05F,
    -3.98975753e-05F,
    -3.90947316e-05F,
    -3.82999060e-05F,
    -3.75130840e-05F,
    -3.67342473e-05F,
    -3.59633814e-05F,
    -3.52004572e-05F,
    -3.44454638e-05F,
    -3.36983758e-05F,
    -3.29591676e-05F,
    -3.22278174e-05F,
    -3.15043035e-05F,
    -3.07886003e-05F,
    -3.00806860e-05F,
    -2.93805333e-05F,
    -2.86881150e-05F,
    -2.80034019e-05F,
    -2.73263649e-05F,
    -2.66569768e-05F,
    -2.59952067e-05F,
    -2.53410235e-05F,
    -2.46943964e-05F,
    -2.40552927e-05F,
    -2.34236795e-05F,
    -2.27995261e-05F,
    -2.21827977e-05F,
    -2.15734581e-05F,
    -2.09714781e-05F,
    -2.03768122e-05F,
    -1.97894333e-05F,
    -1.92093030e-05F,
    -1.86363832e-05F,
    -1.80706411e-05F,
    -1.75120331e-05F,
    -1.69605228e-05F,
    -1.64160720e-05F,
    -1.58786388e-05F,
    -1.53481888e-05F,
    -1.48246781e-05F,
    -1.43080679e-05F,
    -1.37983170e-05F,
    -1.32953865e-05F,
    -1.27992325e-05F,
    -1.23098152e-05F,
    -1.18270909e-05F,
    -1.13510196e-05F,
    -1.08815548e-05F,
    -1.04186556e-05F,
    -9.96227936e-06F,
    -9.51238053e-06F,
    -9.06891728e-06F,
    -8.63184596e-06F,
    -8.20112109e-06F,
    -7.77669993e-06F,
    -7.35853791e-06F,
    -6.94658866e-06F,
    -6.54080759e-06F,
    -6.14115106e-06F,
    -5.74757223e-06F,
    -5.36002653e-06F,
    -4.97846713e-06F,
    -4.60284991e-06F,
    -4.23312804e-06F,
    -3.86925558e-06F,
    -3.51118661e-06F,
    -3.15887542e-06F,
    -2.81227494e-06F,
    -2.47133949e-06F,
    -2.13602243e-06F,
    -1.80627774e-06F,
    -1.48205834e-06F,
    -1.16331807e-06F,
    -8.50009997e-07F,
    -5.42087662e-07F,
    -2.39504431e-07F,
    5.77864441e-08F,
    3.49831623e-07F,
    6.36677726e-07F,
    9.18371484e-07F,
    1.19495951e-06F,
    1.46648836e-06F,
    1.73300486e-06F,
    1.99455530e-06F,
    2.25118629e-06F,
    2.50294465e-06F,
    2.74987656e-06F,
    2.99202816e-06F,
    3.22944629e-06F,
    3.46217666e-06F,
    3.69026520e-06F,
    3.91375806e-06F,
    4.13270163e-06F,
    4.34714138e-06F,
    4.55712325e-06F,
    4.76269315e-06F,
    4.96389612e-06F,
    5.16077762e-06F,
    5.35338268e-06F,
    5.54175631e-06F,
    5.72594354e-06F,
    5.90598984e-06F,
    6.08194023e-06F,
    6.25383882e-06F,
    6.42173063e-06F,
    6.58565887e-06F,
    6.74566900e-06F,
    6.90180377e-06F,
    7.05410821e-06F,
    7.20262506e-06F,
    7.34739842e-06F,
    7.48847106e-06F,
    7.62588661e-06F,
    7.75968692e-06F,
    7.88991565e-06F,
    8.01661554e-06F,
    8.13982842e-06F,
    8.25959705e-06F,
    8.37596144e-06F,
    8.48896525e-06F,
    8.59864849e-06F,
    8.70505392e-06F,
    8.80822336e-06F,
    8.90819501e-06F,
    9.00501163e-06F,
    9.09871233e-06F,
    9.18933711e-06F,
    9.27692781e-06F,
    9.36152264e-06F,
    9.44316071e-06F,
    9.52188293e-06F,
    9.59772751e-06F,
    9.67073356e-06F,
    9.74093746e-06F,
    9.80838013e-06F,
    9.87309886e-06F,
    9.93513186e-06F,
    9.99451549e-06F,
    1.00512880e-05F,
    1.01054848e-05F,
    1.01571450e-05F,
    1.02063059e-05F,
    1.02530030e-05F,
    1.02972708e-05F,
    1.03391458e-05F,
    1.03786642e-05F,
    1.04158626e-05F,
    1.04507744e-05F,
    1.04834362e-05F,
    1.05138806e-05F,
    1.05421414e-05F,
    1.05682557e-05F,
    1.05922536e-05F,
    1.06141706e-05F,
    1.06340403e-05F,
    1.06518946e-05F,
    1.06677662e-05F,
    1.06816879e-05F,
    1.06936923e-05F,
    1.07038113e-05F,
    1.07120759e-05F,
    1.07185188e-05F,
    1.07231699e-05F,
    1.07260612e-05F,
    1.07272208e-05F,
    1.07266787e-05F,
    1.07244687e-05F,
    1.07206179e-05F,
    1.07151573e-05F,
    1.07081150e-05F,
    1.06995212e-05F,
    1.06894040e-05F,
    1.06777907e-05F,
    1.06647094e-05F,
    1.06501884e-05F,
    1.06342559e-05F,
    1.06169373e-05F,
    1.05982617e-05F,
    1.05782556e-05F,
    1.05569452e-05F,
    1.05343570e-05F,
    1.05105164e-05F,
    1.04854516e-05F,
    1.04591854e-05F,
    1.04317460e-05F,
    1.04031569e-05F,
    1.03734428e-05F,
    1.03426282e-05F,
    1.03107368e-05F,
    1.02777922e-05F,
    1.02438189e-05F,
    1.02088397e-05F,
    1.01728801e-05F,
    1.01359610e-05F,
    1.00981051e-05F,
    1.00593352e-05F,
    1.00196721e-05F,
    9.97913776e-06F,
    9.93775575e-06F,
    9.89554610e-06F,
    9.85253064e-06F,
    9.80873119e-06F,
    9.76416686e-06F,
    9.71885856e-06F,
    9.67282540e-06F,
    9.62609010e-06F,
    9.57866996e-06F,
    9.53058588e-06F,
    9.48185516e-06F,
    9.43249870e-06F,
    9.38253470e-06F,
    9.33198226e-06F,
    9.28085865e-06F,
    9.22918298e-06F,
    9.17697162e-06F,
    9.12424366e-06F,
    9.07101457e-06F,
    9.01730346e-06F,
    8.96312667e-06F,
    8.90850060e-06F,
    8.85344161e-06F,
    8.79796607e-06F,
    8.74208854e-06F,
    8.68582720e-06F,
    8.62919660e-06F,
    8.57221130e-06F,
    8.51488767e-06F,
    8.45724117e-06F,
    8.39928543e-06F,
    8.34103503e-06F,
    8.28250541e-06F,
    8.22370930e-06F,
    8.16466127e-06F,
    8.10537495e-06F,
    8.04586398e-06F,
    7.98614201e-06F,
    7.92622086e-06F,
    7.86611508e-06F,
    7.80583650e-06F,
    7.74539694e-06F,
    7.68480913e-06F,
    7.62408399e-06F,
    7.56323561e-06F,
    7.50227400e-06F,
    7.44121098e-06F,
    7.38005838e-06F,
    7.31882710e-06F,
    7.25752807e-06F,
    7.19617219e-06F,
    7.13476902e-06F,
    7.07333083e-06F,
    7.01186718e-06F,
    6.95038761e-06F,
    6.88890350e-06F,
    6.82742348e-06F,
    6.76595755e-06F,
    6.70451527e-06F,
    6.64310573e-06F,
    6.58173849e-06F,
    6.52042127e-06F,
    6.45916316e-06F,
    6.39797281e-06F,
    6.33685886e-06F,
    6.27582813e-06F,
    6.21489107e-06F,
    6.15405315e-06F,
    6.09332346e-06F,
    6.03271019e-06F,
    5.97221924e-06F,
    5.91185926e-06F,
    5.85163662e-06F,
    5.79155767e-06F,
    5.73163061e-06F,
    5.67186180e-06F,
    5.61225716e-06F,
    5.55282304e-06F,
    5.49356719e-06F,
    5.43449460e-06F,
    5.37561118e-06F,
    5.31692422e-06F,
    5.25843780e-06F,
    5.20015783e-06F,
    5.14209023e-06F,
    5.08424091e-06F,
    5.02661396e-06F,
    4.96921484e-06F,
    4.91204946e-06F,
    4.85512146e-06F,
    4.79843584e-06F,
    4.74199805e-06F,
    4.68581129e-06F,
    4.62988100e-06F,
    4.57421038e-06F,
    4.51880442e-06F,
    4.46366630e-06F,
    4.40880103e-06F,
    4.35421180e-06F,
    4.29990223e-06F,
    4.24587643e-06F,
    4.19213757e-06F,
    4.13868838e-06F,
    4.08553296e-06F,
    4.03267359e-06F,
    3.98011389e-06F,
    3.92785660e-06F,
    3.87590444e-06F,
    3.82426151e-06F,
    3.77292872e-06F,
    3.72190993e-06F,
    3.67120651e-06F,
    3.62082164e-06F,
    3.57075714e-06F,
    3.52101574e-06F,
    3.47159903e-06F,
    3.42250905e-06F,
    3.37374809e-06F,
    3.32531727e-06F,
    3.27721864e-06F,
    3.22945380e-06F,
    3.18202478e-06F,
    3.13493274e-06F,
    3.08817971e-06F,
    3.04176615e-06F,
    2.99569410e-06F,
    2.94996448e-06F,
    2.90457820e-06F,
    2.85953683e-06F,
    2.81484085e-06F,
    2.77049162e-06F,
    2.72648958e-06F,
    2.68283543e-06F,
    2.63952984e-06F,
    2.59657418e-06F,
    2.55396867e-06F,
    2.51171377e-06F,
    2.46980994e-06F,
    2.42825718e-06F,
    2.38705570e-06F,
    2.34620643e-06F,
    2.30570913e-06F,
    2.26556426e-06F,
    2.22577160e-06F,
    2.18633159e-06F,
    2.14724378e-06F,
    2.10850840e-06F,
    2.07012499e-06F,
    2.03209356e-06F,
    1.99441388e-06F,
    1.95708571e-06F,
    1.92010839e-06F,
    1.88348190e-06F,
    1.84720557e-06F,
    1.81127893e-06F,
    1.77570155e-06F,
    1.74047284e-06F,
    1.70559224e-06F,
    1.67105895e-06F,
    1.63687241e-06F,
    1.60303159e-06F,
    1.56953604e-06F,
    1.53638462e-06F,
    1.50357653e-06F,
    1.47111086e-06F,
    1.43898660e-06F,
    1.40720294e-06F,
    1.37575864e-06F,
    1.34465279e-06F,
    1.31388424e-06F,
    1.28345198e-06F}};

std::array<double, 301> pid_closed_loop_response{{
    0.0,
    0.05169525,
    0.155672803,
    0.258214386,
    0.353902124,
    0.440000413,
    0.516798144,
    0.585011859,
    0.645495853,
    0.699065252,
    0.746466063,
    0.788369011,
    0.825374,
    0.858016525,
    0.886774269,
    0.912073232,
    0.934293284,
    0.953773141,
    0.970814814,
    0.985687569,
    0.998631477,
    1.009860562,
    1.019565625,
    1.02791675,
    1.035065547,
    1.041147151,
    1.046282003,
    1.050577438,
    1.054129106,
    1.057022236,
    1.05933276,
    1.061128323,
    1.06246918,
    1.063408993,
    1.063995549,
    1.064271393,
    1.064274396,
    1.064038264,
    1.063592987,
    1.062965243,
    1.062178756,
    1.061254619,
    1.060211576,
    1.059066279,
    1.057833517,
    1.056526414,
    1.055156614,
    1.05373444,
    1.052269039,
    1.05076851,
    1.049240016,
    1.04768989,
    1.046123723,
    1.044546446,
    1.042962401,
    1.041375409,
    1.039788823,
    1.038205584,
    1.03662826,
    1.035059095,
    1.03350004,
    1.031952784,
    1.030418789,
    1.028899311,
    1.027395423,
    1.025908038,
    1.024437924,
    1.022985725,
    1.021551969,
    1.020137088,
    1.018741423,
    1.017365238,
    1.016008729,
    1.01467203,
    1.013355224,
    1.012058344,
    1.010781384,
    1.009524301,
    1.008287021,
    1.007069441,
    1.005871435,
    1.004692857,
    1.003533542,
    1.002393309,
    1.001271967,
    1.000169309,
    0.999085122,
    0.998019184,
    0.996971267,
    0.995941137,
    0.994928557,
    0.993933285,
    0.992955078,
    0.991993691,
    0.991048877,
    0.990120392,
    0.989207987,
    0.988311418,
    0.98743044,
    0.986564808,
    0.985714282,
    0.984878621,
    0.984057586,
    0.983250942,
    0.982458455,
    0.981679895,
    0.980915033,
    0.980163643,
    0.979425504,
    0.978700394,
    0.977988097,
    0.977288399,
    0.976601089,
    0.97592596,
    0.975262806,
    0.974611426,
    0.97397162,
    0.973343194,
    0.972725954,
    0.972119711,
    0.971524278,
    0.970939471,
    0.97036511,
    0.969801017,
    0.969247016,
    0.968702936,
    0.968168607,
    0.967643862,
    0.967128539,
    0.966622475,
    0.966125513,
    0.965637497,
    0.965158274,
    0.964687692,
    0.964225604,
    0.963771865,
    0.963326332,
    0.962888863,
    0.96245932,
    0.962037569,
    0.961623473,
    0.961216904,
    0.960817731,
    0.960425827,
    0.960041068,
    0.959663331,
    0.959292495,
    0.958928442,
    0.958571056,
    0.958220222,
    0.957875827,
    0.957537761,
    0.957205915,
    0.956880183,
    0.956560458,
    0.956246639,
    0.955938623,
    0.955636311,
    0.955339604,
    0.955048407,
    0.954762625,
    0.954482165,
    0.954206934,
    0.953936844,
    0.953671806,
    0.953411733,
    0.95315654,
    0.952906142,
    0.952660458,
    0.952419406,
    0.952182907,
    0.951950883,
    0.951723255,
    0.95149995,
    0.951280892,
    0.951066009,
    0.950855228,
    0.950648479,
    0.950445693,
    0.950246801,
    0.950051737,
    0.949860435,
    0.94967283,
    0.949488858,
    0.949308457,
    0.949131566,
    0.948958123,
    0.94878807,
    0.948621349,
    0.948457902,
    0.948297672,
    0.948140604,
    0.947986645,
    0.94783574,
    0.947687836,
    0.947542883,
    0.947400829,
    0.947261624,
    0.94712522,
    0.946991568,
    0.946860621,
    0.946732332,
    0.946606655,
    0.946483545,
    0.946362959,
    0.946244853,
    0.946129183,
    0.946015909,
    0.945904988,
    0.94579638,
    0.945690046,
    0.945585946,
    0.945484041,
    0.945384295,
    0.945286668,
    0.945191126,
    0.945097633,
    0.945006152,
    0.944916649,
    0.94482909,
    0.944743441,
    0.94465967,
    0.944577745,
    0.944497632,
    0.944419302,
    0.944342723,
    0.944267865,
    0.944194699,
    0.944123195,
    0.944053324,
    0.943985058,
    0.943918371,
    0.943853233,
    0.94378962,
    0.943727503,
    0.943666858,
    0.943607659,
    0.943549882,
    0.9434935,
    0.943438491,
    0.943384831,
    0.943332496,
    0.943281464,
    0.943231712,
    0.943183217,
    0.943135959,
    0.943089915,
    0.943045065,
    0.943001388,
    0.942958864,
    0.942917473,
    0.942877195,
    0.942838011,
    0.942799902,
    0.942762849,
    0.942726835,
    0.942691841,
    0.942657849,
    0.942624842,
    0.942592804,
    0.942561717,
    0.942531564,
    0.942502331,
    0.942474,
    0.942446556,
    0.942419983,
    0.942394268,
    0.942369394,
    0.942345347,
    0.942322112,
    0.942299677,
    0.942278026,
    0.942257146,
    0.942237025,
    0.942217647,
    0.942199002,
    0.942181075,
    0.942163855,
    0.942147329,
    0.942131485,
    0.942116311,
    0.942101796,
    0.942087928,
    0.942074695,
    0.942062088,
    0.942050094,
    0.942038704,
    0.942027907,
    0.942017691,
    0.942008048,
    0.941998968,
    0.941990439,
    0.941982454,
    0.941975002,
    0.941968074,
    0.94196166,
    0.941955753,
    0.941950343,
    0.941945421,
    0.941940979,
    0.941937008}};

}  // namespace toptica::test::data
