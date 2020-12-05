% MATLAB test case for RetroArch SINC upsampler.
close all;

%%
% Test RetroArch's kaiser function.
real_kaiser = kaiser(1024, 10.0)';
rarch_kaiser = kaiser_window(1024, 10.0);
figure('name', 'Bessel function test');
subplot(2, 1, 1), plot(rarch_kaiser), title('RetroArch kaiser');
subplot(2, 1, 2), plot(rarch_kaiser - real_kaiser), title('Error');

%%
% 4-tap and 8-tap are Lanczos windowed, but include here for completeness.
phases = 256;
ratio = 2.0;
bw = min(1.0, ratio);
downsample = round(phases / ratio);
cutoffs = bw * [0.65 0.75 0.825 0.90 0.962];
betas = [2.0 3.0 5.5 10.5 14.5];

sidelobes = round([2 4 8 32 128] / bw);
taps = sidelobes * 2;

freqs = 0.05 : 0.02 : 0.99;

%filters = length(taps);
%for i = 1 : filters
for i = 5
    filter_length = taps(i) * phases;
    
    % Generate SINC.
    sinc_indices = 2 * ((0 : (filter_length - 1)) / filter_length) - 1;
    s = cutoffs(i) * sinc(cutoffs(i) * sinc_indices * sidelobes(i));
    win = kaiser(filter_length, betas(i))';
    filter = s .* win;

    impulse_response_half = upfirdn(1, filter, phases, downsample) / bw;
    figure('name', sprintf('Response SINC: %d taps', taps(i)));
    freqz(impulse_response_half);
    ylim([-200 0]);

    signal = zeros(1, 80001);
    for freq = freqs
        signal = signal + sin(pi * freq * (0 : 80000));
    end
    
    resampled = upfirdn(signal, filter, phases, downsample);
    figure('name', sprintf('Kaiser SINC: %d taps, w = %.f', taps(i), freq));
    freqz(resampled .* kaiser(length(resampled), 40.0)', 1, 16 * 1024);
    ylim([-180 100]);
end